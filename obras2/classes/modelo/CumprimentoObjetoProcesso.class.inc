<?php

/**
 * Description of CumprimentoObjetoProcesso
 *
 * @author lindalbertofilho
 */
class CumprimentoObjetoProcesso extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_processo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("copid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'copid' => null,
        'numprocesso' => null,
        'arqid_pf' => null,
        'arqid_pf_revisado' => null,
        'valor_devolucao' => null,
        'aprovacao' => null,
        'extrato_parecer' => null,
    );

    public function salvarConclusao($dados)
    {
        if ($dados['novaPactuacao'] == 14) {
            $nova_pactuacao = 'S';
            if (!$dados['valorNovaPac']) {
                $dados['vpndtinclusao'] = "now()";
                $dados['vpncpfinclusao'] = $_SESSION["usucpf"];
                $dados['novoValorBase'] = str_replace(',', '.', str_replace('.', '', $dados['novoValorBase']));
                $dados['novoExecucao'] = str_replace('%', '', $dados['novoExecucao']);
                $dados['novoExecucao'] = str_replace(',', '.', str_replace('.', '', $dados['novoExecucao']));
                $dados['valorFNDE'] = str_replace(',', '.', str_replace('.', '', $dados['valorFNDE']));
                $dados['contrapartida'] = str_replace(',', '.', str_replace('.', '', $dados['contrapartida']));
                $dados['saldoEmConta'] = str_replace(',', '.', str_replace('.', '', $dados['saldoEmConta']));
                $dados['dataBase'] = formata_data_sql($dados['dataBase']);

                $dados['preid'] = $dados['preid'] != '' ? $dados['preid'] : 'null';
                $dados['docid'] = $dados['docid'] != '' ? $dados['docid'] : 'null';
                $dados['novoValorBase'] = $dados['novoValorBase'] != '' ? $dados['novoValorBase'] : 'null';
                $dados['novoExecucao'] = $dados['novoExecucao'] != '' ? $dados['novoExecucao'] : 'null';
                $dados['valorFNDE'] = $dados['valorFNDE'] != '' ? $dados['valorFNDE'] : 'null';
                $dados['contrapartida'] = $dados['contrapartida'] != '' ? $dados['contrapartida'] : 'null';
                $dados['saldoEmConta'] = $dados['saldoEmConta'] != '' ? $dados['saldoEmConta'] : 'null';
                $dados['dataBase'] = $dados['dataBase'] != '' ? "'" . $dados['dataBase'] . "'" : 'null';
            }
        } else {
            $nova_pactuacao = 'N';
        }

        $estadoWorkflowObra = wf_pegarEstadoAtual($dados['docid']);


        $file = new FilesSimec('arquivo', null, 'obras2');
        $file->setPasta('obras2');

        // -- Tratando caracteres especiais do extrato_parecer
        $dados['extrato_parecer'] = filter_input(INPUT_POST, 'extrato_parecer', FILTER_SANITIZE_ENCODED);


        if ($_POST['copid'] && $nova_pactuacao == 'N') {

            $setExtra = '';
            if ($_FILES['parecerfinal']) {
                $file->setUpload('', 'parecerfinal', false);
                $arqidParecerFinal = $file->getIdArquivo();
                $setExtra .= ', arqid_pf = ' . $arqidParecerFinal;
            }
            if ($_FILES['parecerfinalrevisado'] && $_FILES['parecerfinalrevisado']['error'] == 0 && $_FILES['parecerfinalrevisado']['size'] > 0) {
                $file->setUpload('', 'parecerfinalrevisado', false);
                $arqidParecerFinalRevisado = $file->getIdArquivo();
                $setExtra .= ', arqid_pf_revisado = ' . $arqidParecerFinalRevisado;
            }

            $sql = <<<DML
                    UPDATE {$this->stNomeTabela}
                      SET aprovacao = '{$dados['aprovacao']}',
                          valor_devolucao = {$dados['valor_devolucao']}, 
                          extrato_parecer = '{$dados['extrato_parecer']}'
                          {$setExtra},
                          nova_pactuacao = '{$nova_pactuacao}'
                      WHERE numprocesso = '{$dados['numprocesso']}' AND copid = {$dados['copid']} 
DML;
            $this->executar($sql);
            $this->commit();
        } elseif (!$_POST['copid'] && $nova_pactuacao == 'N') {
            
            $file->setUpload('', 'parecerfinal', false);
            $arqidParecerFinal = $file->getIdArquivo();
            $arqidParecerFinalRevisado = 'NULL';
            if ($_FILES['parecerfinalrevisado'] && $_FILES['parecerfinalrevisado']['error'] == 0 && $_FILES['parecerfinalrevisado']['size'] > 0) {
                $file->setUpload('', 'parecerfinalrevisado', false);
                $arqidParecerFinalRevisado = $file->getIdArquivo();
            }
            $dados['valor_devolucao'] = (empty($dados['valor_devolucao'])) ? '0.00' : $dados['valor_devolucao'];
            
            
            $sql = <<<DML
                         INSERT INTO {$this->stNomeTabela}(numprocesso, arqid_pf, arqid_pf_revisado, valor_devolucao, aprovacao, extrato_parecer,nova_pactuacao)
                          VALUES (
                              '{$dados['numprocesso']}',
                              '{$arqidParecerFinal}',
                              {$arqidParecerFinalRevisado},
                              {$dados['valor_devolucao']},
                              '{$dados['aprovacao']}',
                              '{$dados['extrato_parecer']}',
                              '{$nova_pactuacao}'
                        );
DML;
            $this->executar($sql);
            $this->commit();
        } elseif ($_POST['copidNovaPactuacao'] && $nova_pactuacao == 'S') {

            $setExtra = '';
            if ($_FILES['parecerNovaPactuacao']) {
                $file->setUpload('', 'parecerNovaPactuacao', false);
                $arqidParecerFinal = $file->getIdArquivo();
                $setExtra .= ', arqid_pf = ' . $arqidParecerFinal;
            }
            if ($_FILES['parecerfinalrevisado'] && $_FILES['parecerfinalrevisado']['error'] == 0 && $_FILES['parecerfinalrevisado']['size'] > 0) {
                $file->setUpload('', 'parecerfinalrevisado', false);
                $arqidParecerFinalRevisado = $file->getIdArquivo();
                $setExtra .= ', arqid_pf_revisado = ' . $arqidParecerFinalRevisado;
            }

            $sql = <<<DML
                    UPDATE {$this->stNomeTabela}
                      SET aprovacao = '{$dados['aprovacao']}',
                          valor_devolucao = {$dados['valor_devolucao']},
                          extrato_parecer = '{$dados['extrato_parecer']}'
                          {$setExtra},
                           nova_pactuacao = '{$nova_pactuacao}'
                      WHERE numprocesso = '{$dados['numprocesso']}' AND copid = {$dados['copidNovaPactuacao']} 
DML;

                          
            if (!$dados['valorNovaPac'] && $estadoWorkflowObra != ESDID_CUMPRIMENTO_APROVADO) {
                
                $consulta = "SELECT vpnid FROM obras2.valorparecernovapac WHERE obrid = {$dados['obrid']} AND vpnstatus = 'A'";
                
                $rs = $this->pegaUm($consulta);

                if(!$rs){
                    $sql1 =
                    "INSERT INTO obras2.valorparecernovapac
                            (obrid,
                            preid,
                            vpnvalorobra,
                            vpnvalorfnde,
                            vpncontrapartida,
                            
                            vpnsaldoconta,
                            vpndtbase,
                            vpnexecucao,
                            
                            vpndtinclusao,
                            vpncpfinclusao)
                      VALUES({$dados['obrid']},
                             {$dados['preid']},
                             {$dados['novoValorBase']},
                             {$dados['valorFNDE']},
                             {$dados['contrapartida']},
                             
                             {$dados['saldoEmConta']},
                             {$dados['dataBase']},
                             {$dados['novoExecucao']},
                             
                             'now()',
                             '{$_SESSION['usucpf']}'
                      )";
                             
                    $this->executar($sql1);
                }else{
                    $sql1 = "
                    UPDATE  obras2.valorparecernovapac
                        SET vpnvalorobra = {$dados['novoValorBase']},
                            vpnvalorfnde = {$dados['valorFNDE']},
                            vpncontrapartida = {$dados['contrapartida']},
                            
                            vpnsaldoconta = {$dados['saldoEmConta']},
                            vpndtbase = {$dados['dataBase']},
                            vpnexecucao = {$dados['novoExecucao']},
                            
                            vpndtalteracao = 'now()',
                            vpncpfalteracao = '{$_SESSION['usucpf']}'
                    WHERE obrid = {$dados['obrid']} AND vpnstatus = 'A'";
                    
                    $this->executar($sql1);
                }
                
            }

            if ($this->executar($sql)) {
                $this->commit();
            }


        } elseif (!$_POST['copidNovaPactuacao'] && $nova_pactuacao == 'S') {

            if ($_FILES['parecerNovaPactuacao'] != '') {
                $file->setUpload('', 'parecerNovaPactuacao', false);
                $arqidParecerFinalNovaPactuacao = $file->getIdArquivo();
                $arqidParecerFinalRevisado = 'NULL';
                if ($_FILES['parecerfinalrevisado'] && $_FILES['parecerfinalrevisado']['error'] == 0 && $_FILES['parecerfinalrevisado']['size'] > 0) {
                    $file->setUpload('', 'parecerfinalrevisado', false);
                    $arqidParecerFinalRevisado = $file->getIdArquivo();
                }
                $dados['valor_devolucao'] = (empty($dados['valor_devolucao'])) ? '0.00' : $dados['valor_devolucao'];
                $sql = <<<DML
                         INSERT INTO {$this->stNomeTabela}(numprocesso, arqid_pf, arqid_pf_revisado, valor_devolucao, aprovacao, extrato_parecer,nova_pactuacao)
                          VALUES (
                              '{$dados['numprocesso']}',
                              '{$arqidParecerFinalNovaPactuacao}',
                              {$arqidParecerFinalRevisado},
                              {$dados['valor_devolucao']},
                              '{$dados['aprovacao']}',
                              '{$dados['extrato_parecer']}',
                              '{$nova_pactuacao}'
                        );
DML;

                if (!$dados['valorNovaPac'] && $estadoWorkflowObra != ESDID_CUMPRIMENTO_APROVADO) {
                    $sql1 =
                        "INSERT INTO obras2.valorparecernovapac
                            (obrid,
                            preid,
                            vpnvalorobra,
                            vpnvalorfnde,
                            vpncontrapartida,
            
                            vpnsaldoconta,
                            vpndtbase,
                            vpnexecucao,
                            
                            vpndtinclusao,
                            vpncpfinclusao)
                      VALUES({$dados['obrid']},
                             {$dados['preid']},
                             {$dados['novoValorBase']},
                             {$dados['valorFNDE']},
                             {$dados['contrapartida']},
                             
                             {$dados['saldoEmConta']},
                             {$dados['dataBase']},
                             {$dados['novoExecucao']},
                             
                             'now()',
                             '{$_SESSION['usucpf']}'
                      )";

                    $this->executar($sql1);
                }
                if ($this->executar($sql)) {
                    $this->commit();
                }

            }
        }


    }

    public function capturaDados($numprocesso, $teste = false)
    {
        $sql = <<<DML
            SELECT
                cop.copid,
                cop.arqid_pf,
                cop.arqid_pf_revisado,
                cop.valor_devolucao,
                cop.aprovacao,
                arq.arqnome AS pfnome,
                arq.arqextensao AS pfextensao,
                arq.usucpf AS pfusu,
                usu.usunome AS pfusunome,
                TO_CHAR(arq.arqdata,'DD/MM/YYYY') AS pfdata,
                arq2.arqnome AS pfrnome,
                arq2.arqextensao AS pfrextensao,
                arq2.usucpf AS pfrusu,
                usu2.usunome AS pfrusunome,
                TO_CHAR(arq2.arqdata,'DD/MM/YYYY') AS pfrdata,
                cop.extrato_parecer,
                cop.nova_pactuacao
            FROM {$this->stNomeTabela} cop
            LEFT JOIN public.arquivo arq ON (cop.arqid_pf = arq.arqid)
            LEFT JOIN seguranca.usuario usu ON (arq.usucpf = usu.usucpf)
            LEFT JOIN public.arquivo arq2 ON (cop.arqid_pf_revisado = arq2.arqid)
            LEFT JOIN seguranca.usuario usu2 ON (arq2.usucpf = usu2.usucpf)
            WHERE cop.numprocesso = '$numprocesso';
DML;

        $dados = $this->pegaLinha($sql);
        $this->popularDadosObjeto($dados);
        return $dados;
    }


    public function capturaDadosArray($numprocesso)
    {
        $sql = <<<DML
            SELECT
                cop.copid,
                cop.arqid_pf,
                cop.arqid_pf_revisado,
                cop.valor_devolucao,
                cop.aprovacao,
                arq.arqnome AS pfnome,
                arq.arqextensao AS pfextensao,
                arq.usucpf AS pfusu,
                usu.usunome AS pfusunome,
                TO_CHAR(arq.arqdata,'DD/MM/YYYY') AS pfdata,
                arq.arqdata AS dataCompara,
                arq.arqhora AS horaCompara,
                arq2.arqnome AS pfrnome,
                arq2.arqextensao AS pfrextensao,
                arq2.usucpf AS pfrusu,
                usu2.usunome AS pfrusunome,
                TO_CHAR(arq2.arqdata,'DD/MM/YYYY') AS pfrdata,
                cop.extrato_parecer,
                cop.nova_pactuacao
            FROM {$this->stNomeTabela} cop
            LEFT JOIN public.arquivo arq ON (cop.arqid_pf = arq.arqid)
            LEFT JOIN seguranca.usuario usu ON (arq.usucpf = usu.usucpf)
            LEFT JOIN public.arquivo arq2 ON (cop.arqid_pf_revisado = arq2.arqid)
            LEFT JOIN seguranca.usuario usu2 ON (arq2.usucpf = usu2.usucpf)
            WHERE cop.numprocesso = '$numprocesso';
DML;

        $dados = $this->carregar($sql);
        // $this->popularDadosObjeto($dados);
        return $dados;
    }

    public function excluir($arqid, $colunaArqid)
    {
        if (!$arqid || !$colunaArqid) {
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        $sql = <<<DML
            UPDATE {$this->stNomeTabela} SET $colunaArqid = NULL WHERE $colunaArqid = $arqid;
DML;
        $this->executar($sql);
        $this->commit();
        $file = new FilesSimec('cumprimento_objeto_processo', $this->arAtributos, 'obras2');
        $file->setPulaTableEschema(true);
        $file->setRemoveUpload($arqid);
    }

    public function verificaObrasPorProcesso($processo, $recuperaObras = true)
    {
        $sql = <<<DML
            SELECT
                CASE WHEN
                    (SELECT count(*) FROM obras2.obras WHERE obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo')) < (SELECT count(*) FROM obras2.obras obr INNER JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid) INNER JOIN obras2.cumprimento_objeto_conclusao coc ON (co.coid = coc.coid) WHERE obr.obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo' ORDER BY obrid))
                    THEN '0'
                    ELSE '1'
                    END
DML;
        $result = $this->pegaUm($sql);
        if ($recuperaObras) {
            $obras = $this->retornaObrasPorProcesso($processo);
        }
        return array('result' => $result == '1' ? true : false, 'obras' => $obras);
    }

    public function retornaObrasPorProcesso($processo)
    {
        $sql = <<<DML
            SELECT obr.obrid, co.docid, co.coid FROM obras2.obras obr INNER JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid) WHERE obr.obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo' ORDER BY obrid);
DML;
        return $this->carregar($sql);
    }

    public function verificaObrasPorProcessoCondicao($processo, $coid)
    {
        $sql = <<<DML
            SELECT
                CASE WHEN
                    (SELECT count(*) FROM obras2.obras WHERE obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo')) <> (SELECT count(*) FROM obras2.obras obr INNER JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid) INNER JOIN workflow.documento doc ON (co.docid = doc.docid) WHERE obr.obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo' ORDER BY obrid) AND doc.esdid = (select esdid from workflow.documento where docid = (select docid from obras2.cumprimento_objeto where coid = $coid)))
                    THEN '0'
                    ELSE '1'
                    END
DML;
        return $this->pegaUm($sql) == '0' ? false : true;
    }
}