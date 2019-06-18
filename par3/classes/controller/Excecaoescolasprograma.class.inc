<?php
/**
 * Classe de controle do  excecaoescolasprograma
 * @category Class
 * @package  A1
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-11-2016
 * @link     no link
 */


/**
 * Par3_Controller_Excecaoescolasprograma
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-11-2016
 * @link     no link
 */
class Par3_Controller_Excecaoescolasprograma
{

    public function salvaFormulario($dados)
    {
        $objExcecao = new Par3_Model_Excecaoescolasprograma();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $dadosUnidade = $modelInstrumentoUnidade->getDadosUnidade($dados['inuid']);

        $inuid = $dados['inuid'];
        $prgid = $dados['prgid'];
        $adpid = $dados['adpid'];

        $objRespostaCarga = new Par3_Model_Respostacargahorariaprograma;
        // Valida se existe carga para o grupo,caso não exista insere
        if (is_array($dados['grupo_escola'])) {
            $arrVerificado = array();
            foreach ($dados['grupo_escola'] as $k => $v) {
                if (!(in_array($v, $arrVerificado))) {
                    $grupo = $v;

                    $dados['adpid'] = $adpid;
                    $dados['inuid'] = $inuid;
                    $dados['prgid'] = $prgid;
                    $dados['grupo'] = $grupo;
                    $resposta = $objRespostaCarga->getResposta($dados);
                    if (!$resposta) {

                        $dadosResposta['rchresposta'] = 'N';
                        $dadosResposta['inuid'] = $inuid;
                        $dadosResposta['adpid'] = $adpid;
                        $dadosResposta['prgid'] = $prgid;
                        $dadosResposta['rchgrupo'] = $grupo;
                        $dadosResposta['rchcpf'] = $_SESSION['usucpf'];

                        $objRespostaCarga->popularDadosObjeto($dadosResposta);
                        $objRespostaCarga->salvar();
                    }
                }
                $arrVerificado[] = $v;
            }
        }

        $insercao = $objExcecao->salvaFormulario($_REQUEST);

        $itrid = $dadosUnidade['itrid'];
        // municipal
        if (($itrid == 2)) {
            $nomeUnidade = "Município de {$dadosUnidade['inudescricao']} - {$dadosUnidade['estuf']}";
        } else {
            $nomeUnidade = "Estado de {$dadosUnidade['inudescricao']}";
        }

        if ($dados['prgid'] == PRGID_PROEMI) {
            $programa = "Ensino Médio Inovador - PROEMI";
            $chaveCarga = 'aepcargahoraria';
            $flag = 'aepinseridaaposenvio';
        } else if ($dados['prgid'] == PRGID_ABCMAIS) {
            $programa = "Novo Mais Educação";
            $chaveCarga = 'aeacargahoraria';
            $flag = 'aeainseridaaposenvio';
        }

        $justificativa = $dados['justificativa'];

        $cargaCount = 0;
        foreach ($dados['escola_checada'] as $k => $v) {
            if ($dados['carga_horaria_' . $codInep] != '') {
                $escolas .= " $v - {$dados['nome_escola'][$v]} - Carga Horária: {$dados['carga_horaria_'.$v]} - Grupo: {$dados['grupo_escola'][$v]} <br>";
                $cargaCount++;
            } else {
                $escolas .= " $v - {$dados['nome_escola'][$v]}  - Grupo: {$dados['grupo_escola'][$v]} <br>";
            }
        }


        $strMensagem =
            "
<div style=\"text-align: justify;\">
O {$nomeUnidade} solicitou a inserção de escolas dentro do programa \"{$programa}\" após o envio ao MEC,
sob a justificativa:<br> \"{$justificativa}\"
</br></br>
Lista de escolas solicitadas:
</br></br> 
{$escolas}

atenciosamente,<br>
Equipe SIMEC

</div>
";

        if ($insercao) {
            // . $dopTexto
            $strAssunto = "Solicitação de inserção de escolas $nomeUnidade";
            $remetente = array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");
            $strMensagem = html_entity_decode($strMensagem);

            if ($_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost") {
                return true;

            } elseif ($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
                $strEmailTo = array('elias.oliveira@mec.gov.br', 'loyane.tavares@mec.gov.br');
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            } else {
                $strEmailTo = array('pddeinterativo@mec.gov.br', 'loyane.tavares@mec.gov.br');
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            }
        } else {
            return false;
        }
    }

    public function salvaFormularioMaisAlfabetizacao($dados)
    {
        global $db;
        $objExcecao = new Par3_Model_Excecaoescolasprograma();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $dadosUnidade = $modelInstrumentoUnidade->getDadosUnidade($dados['inuid']);
        $programa = "Mais Alfabetização";

        // Valida se existe carga para o grupo,caso não exista insere
        $justificativa = $dados['justificativa'];


        $insercao = $objExcecao->salvaFormularioAlfabetizacao($dados);

        $itrid = $dadosUnidade['itrid'];

        if (($itrid == 2)) {
            $nomeUnidade = "Município de {$dadosUnidade['inudescricao']} - {$dadosUnidade['estuf']}";
        } else {
            $nomeUnidade = "Estado de {$dadosUnidade['inudescricao']}";
        }

        $escolas = array();
        if (is_array($dados['escola_checada'])) {
            foreach ($dados['escola_checada'] as $k => $v) {
                $sql = <<<SQL
    SELECT * FROM par3.escolasmaisalfabetizacao WHERE emacodinep = {$v}
SQL;
                $escolaSelecionada = $db->pegaLinha($sql);
                $grupo = ($escolaSelecionada['emacargahoraria'] == 10 ? 1 : 2);
                $escolas .= " $v - {$dados['nome_escola'][$v]} - Carga Horária: {$dados['carga_horaria_'.$v]} - Grupo: {$grupo} / {$escolaSelecionada['emacargahoraria']}h <br>";
            }


            $strMensagem =
                "
<div style=\"text-align: justify;\">
O {$nomeUnidade} solicitou a inserção de escolas dentro do programa \"{$programa}\" após o envio ao MEC,
sob a justificativa:<br> \"{$justificativa}\"
</br></br>
Lista de escolas solicitadas:
</br></br> 
{$escolas}

atenciosamente,<br>
Equipe SIMEC

</div>
";
        }
        if ($insercao) {
            $strAssunto = "Solicitação de inserção de escolas $nomeUnidade";
            $remetente = array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");
            $strMensagem = html_entity_decode($strMensagem);

            if ($_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" || $_SERVER['HTTP_HOST'] == "simec-dsv") {
                return true;
            } elseif ($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
                $strEmailTo = array('junio.santos@mec.gov.br', 'loyane.tavares@mec.gov.br', 'gabrielmoreira@mec.gov.br');
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            } else {
                $strEmailTo = array('pddeinterativo@mec.gov.br', 'junio.santos@mec.gov.br', 'loyane.tavares@mec.gov.br', 'gabrielmoreira@mec.gov.br');
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            }
        } else {
            return false;
        }
    }

    public function alteraStatusSolicitacaoExcecao($dados)
    {
        $objExcecao = new Par3_Model_Excecaoescolasprograma();
        $retorno = $objExcecao->alteraStatusSolicitacaoExcecao($dados);

        return $retorno;
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $excecaoescolasprograma = new Par3_Controller_Excecaoescolasprograma();
        $excecaoescolasprograma->popularDadosObjeto();
        $url .= '&eepid=' . $excecaoescolasprograma->eepid;

        try {
            $sucesso = $excecaoescolasprograma->salvar();
            $excecaoescolasprograma->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if ($sucesso) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }//end salvar()


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['eepid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&eepid={$id}";
        $excecaoescolasprograma = new Par3_Controller_Excecaoescolasprograma($id);
        try {
            $excecaoescolasprograma->Xstatus = 'I';
            $excecaoescolasprograma->Xdtinativacao = date('Y-m-d H:i:s');
            $excecaoescolasprograma->Xusucpfinativacao = $_SESSION['usucpf'];

            $excecaoescolasprograma->salvar();
            $excecaoescolasprograma->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()

    
    public function salvaFormularioEscolaAcessivel($dados)
    {
        global $db;
        $objExcecao = new Par3_Model_Excecaoescolasprograma();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $dadosUnidade = $modelInstrumentoUnidade->getDadosUnidade($dados['inuid']);
        $programa = "Escola Acessível";

        // Valida se existe carga para o grupo,caso não exista insere
        $justificativa = $dados['justificativa'];


        $insercao = $objExcecao->salvaFormularioEA($dados);

        $itrid = $dadosUnidade['itrid'];

        if (($itrid == 2)) {
            $nomeUnidade = "Município de {$dadosUnidade['inudescricao']} - {$dadosUnidade['estuf']}";
        } else {
            $nomeUnidade = "Estado de {$dadosUnidade['inudescricao']}";
        }

        $escolas = array();
        if (is_array($dados['escola_checada'])) {
            foreach ($dados['escola_checada'] as $k => $v) {
                $sql = <<<SQL
    SELECT * FROM par3.escolas_ea WHERE eeacodinep = {$v}
SQL;
                $escolaSelecionada = $db->pegaLinha($sql);
                $grupo = ($escolaSelecionada['eeacargahoraria'] == 10 ? 1 : 2);
                $escolas .= " $v - {$dados['nome_escola'][$v]} - Carga Horária: {$dados['carga_horaria_'.$v]} - Grupo: {$grupo} / {$escolaSelecionada['eeacargahoraria']}h <br>";
            }


            $strMensagem =
                "
<div style=\"text-align: justify;\">
O {$nomeUnidade} solicitou a inserção de escolas dentro do programa \"{$programa}\" após o envio ao MEC,
sob a justificativa:<br> \"{$justificativa}\"
</br></br>
Lista de escolas solicitadas:
</br></br> 
{$escolas}

atenciosamente,<br>
Equipe SIMEC

</div>
";
        }
        if ($insercao) {
            $strAssunto = "Solicitação de inserção de escolas $nomeUnidade";
            $remetente = array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");
            $strMensagem = html_entity_decode($strMensagem);

            if ($_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" || $_SERVER['HTTP_HOST'] == "simec-dsv") {
                return true;
            } elseif ($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
                $strEmailTo = array('junio.santos@mec.gov.br', 'loyane.tavares@mec.gov.br', 'gabrielmoreira@mec.gov.br');
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            } else {
                $strEmailTo = array('pddeinterativo@mec.gov.br', 'junio.santos@mec.gov.br', 'loyane.tavares@mec.gov.br', 'gabrielmoreira@mec.gov.br');
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            }
        } else {
            return false;
        }
    }
}//end Class
?>