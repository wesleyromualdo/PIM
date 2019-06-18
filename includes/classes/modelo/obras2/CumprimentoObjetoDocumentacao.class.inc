<?php
class CumprimentoObjetoDocumentacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_documentacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("codid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'codid' => null,
        'coid' => null,
        'codstatus' => null,
        'usucpf' => null,
        'coddatainclusao' => null,
        'obridvinculado' => null,
        'cexid' => null
    );

    protected $arDados = array(
        'obraAtiva' => null,
        'obraVinculada' => null,
        'construtoraExtra' => null,
    );

    /**
    * Retorna a obra original
    * @param $obrid
    * @return mixed
    */
    private function pegaObraOriginal($obrid)
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid,
                co.coid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A')
            WHERE obr.obrid = {$obrid}
                AND obrstatus IN ('A')
            ";
        $obra = $this->pegaLinha($strSQL);

        if($obra) {
            return $obra;
        }

        $strSQL = "SELECT obrid, obridvinculado FROM obras2.obras WHERE obrid = {$obrid} AND obrstatus IN ('P')";
        $obra = $this->pegaLinha($strSQL);
        return $this->pegaObraOriginal($obra['obridvinculado']);
    }

    /**
     * Pega todas as obras vinculadas
     *
     * @param $obrid
     * @param array $vinculados
     * @return array
     */
    private function pegaObrasVinculadas($obrid, $vinculados = array(), $principal)
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON ($principal = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A' AND obr.obrid = cod.obridvinculado)
            WHERE obr.obridvinculado = {$obrid}
                AND obr.obrstatus IN ('P');
        ";
        $vinculada = $this->pegaLinha($strSQL);

        if ($vinculada) {
            $vinculados[] = $vinculada;
            return $this->pegaObrasVinculadas($vinculada['obrid'], $vinculados, $principal);
        }

        return $vinculados;
    }


    public function verificaExistencia($obrid,$cria = false)
    {
        $strSQL = "
            SELECT cod.codid FROM obras2.cumprimento_objeto co
            INNER JOIN obras2.cumprimento_objeto_documentacao cod ON (cod.coid=co.coid)
            WHERE co.obrid = $obrid";
        $linha = $this->pegaLinha($strSQL);
        if(!$linha['codid']) {
            if($cria){
                if($this->capturaDadosObra($obrid)){
                    $this->criaCodid();
                }
            }
            return false;
        }
        return true;
    }

    /**
     * Utilizada para criar o registro na documentação do cumprimento do objeto. Existem cumprimentos em que a aba de documentação não é salva pelo usuário, impossibilitando a visualização do registro na lista de cumprimento do objeto.
     */
    private function criaCodid()
    {
        if(!$this->arDados['obraAtiva']['codid']) {
            $this->clearDados();
            $this->popularDadosObjeto(
                array('coid' => $this->arDados['obraAtiva']['coid'],
                    'codstatus' => 'A',
                    'usucpf' => $_SESSION['usucpf'],
                    'coddatainclusao' => 'NOW()'));
            $this->inserir();
        }
        if($this->arDados['obraVinculada']) {
            foreach($this->arDados['obraVinculada'] as $obraVinculada) {
                if(!$obraVinculada['codid']) {
                    $this->clearDados();
                    $this->popularDadosObjeto(
                        array('coid' => $this->arDados['obraAtiva']['coid'],
                            'codstatus' => 'A',
                            'usucpf' => $_SESSION['usucpf'],
                            'coddatainclusao' => 'NOW()',
                            'obridvinculado' => $obraVinculada['obrid']));
                    $this->inserir();
                }
            }
        }

        $this->commit();
    }

    private function capturaDadosObra($obrid)
    {
        if(!$obrid) {
            return false;
        }

        $this->arDados['obraAtiva'] = $this->pegaObraOriginal($obrid);
        if(!$this->arDados['obraAtiva']) {
            return false;
        }

        $this->arDados['obraVinculada'] = $this->pegaObrasVinculadas($this->arDados['obraAtiva']['obrid'], array(), $this->arDados['obraAtiva']['obrid']);
        $this->arDados['construtoraExtra'] = $this->pegaConstrutoraExtra($this->arDados['obraAtiva']['obrid']);
        return true;
    }

    public function montaQuestionario($obrid)
    {
        if(!$this->capturaDadosObra($obrid)){
            return false;
        }

        $questionarioCumprimentoObjeto = new QuestionarioCumprimentoObjeto();
        $questionarioCumprimentoObjeto->montaQuestionario($this->arDados['obraAtiva']);
        $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
        $questionarioCumprimentoObjetoConstrutora->montaQuestionario($this->arDados['obraAtiva'],false);
        $questionario = array(
            'questionario' => $questionarioCumprimentoObjeto,
            'questionarioConstrutora' => $questionarioCumprimentoObjetoConstrutora);

        #Verifica a existencia de obras vinculadas.
        
        
        $vinculadas = array();
        if($this->arDados['obraVinculada']) {
            foreach($this->arDados['obraVinculada'] as $obraVinculada) {
                $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
                $vinculadas[] = array(
                    'questionarioConstrutora' => $questionarioCumprimentoObjetoConstrutora->montaQuestionario($obraVinculada, false, false,true),
                );
            }
        }
        
        $questionario['vinculadas'] = $vinculadas;

        #Verifica a existencia de construtoras extras.
        $extras = array();
        if(!empty($this->arDados['construtoraExtra'])) {
            foreach($this->arDados['construtoraExtra'] as $construtoraExtra) {
                $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
                $extras[] = array(
                    'questionarioConstrutoraExtra' => $questionarioCumprimentoObjetoConstrutora->montaQuestionario($construtoraExtra, false, true, false),
                );
            }
        }
        $questionario['extras'] = $extras;

        return $questionario;
    }

    public function salvar($dados)
    {
        $codid = $dados['codid'];
        if (!$codid) {
            $this->popularDadosObjeto(
                array('coid' => $dados['coid'],
                    'codstatus' => 'A',
                    'usucpf' => $_SESSION['usucpf'],
                    'coddatainclusao' => 'NOW()',
                    'obridvinculado' => $dados['obridvinculado'],
                    'cexid' => $dados['cexid']));
            $codid = $this->inserir();
            $this->commit();
        }

        $this->salvarQuestoes($codid, $dados['qcodresposta']);
        return true;
    }

    public function salvarQuestoes($codid, $questoes)
    {
        if(!$codid) {
            return false;
        }

        foreach($questoes as $qstid => $resposta) {
            $questionarioCumprimento = new QuestionarioCumprimentoObjeto();
            $arAtributos = array(
                'qstid' => $qstid,
                'qcodresposta' => ($_FILES[$qstid]['size'] > 0) ? 't' : $resposta,
                'qcodesclarecimento' => $_POST['qcodesclarecimento'][$qstid],
                'qcodobs' => '',
                'qcodstatus' => 'A',
                'cumid' => $codid
            );

            $questionarioCumprimento->popularDadosObjeto($arAtributos);
            $qcodid = $questionarioCumprimento->verificaExistencia();

            if($qcodid) {
                $arAtributos = array('qcodid' => $qcodid);
                $questionarioCumprimento->popularDadosObjeto($arAtributos);
                $questionarioCumprimento->alterar();
                $questionarioCumprimento->commit();
            }else {
                $qcodid = $questionarioCumprimento->inserir();
            }
            $this->insereAnexo($qstid,$qcodid);
        }
    }

    public function insereAnexo($qstid, $qcodid)
    {
        if($_FILES[$qstid]['size'] > 0) {
            $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
            $questaoCumprimentoArquivo->novo($qstid,$qcodid);
        }
    }

    public function validaRespostasDocumentacao($obrid)
    {
        if(!$this->capturaDadosObra($obrid)){
            return false;
        }
        $questionarioCumprimentoObjeto = new QuestionarioCumprimentoObjeto();

        if ($questionarioCumprimentoObjeto->respostas($this->arDados['obraAtiva']) > 0) {
            return false;
        }

        $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
        if ($questionarioCumprimentoObjetoConstrutora->respostas($this->arDados['obraAtiva'],false) > 0) {
            return false;
        }

        if($this->arDados['obraVinculada']) {
            foreach($this->arDados['obraVinculada'] as $obraVinculada) {
                $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
                if($questionarioCumprimentoObjetoConstrutora->respostas($obraVinculada,false, true) > 0) {
                    return false;
                }
            }
        }
        return true;

    }

    /**
     * Função que obtém os dados de uma construtora extra.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
     * @param $obrid ID da obra.
     * @return array|mixed|NULL
     */
    public function pegaConstrutoraExtra($obrid){
        //$strSQL = "SELECT * FROM obras2.construtoraextra ce WHERE ce.obrid = {$obrid};";
        $strSQL = "SELECT
                      cex.cexid,
                      cex.obrid,
                      cex.cexrazsocialconstrutora,
                      cex.cexnumcnpj,
                      cod.codid
                    FROM
                      obras2.construtoraextra cex
                    LEFT JOIN
                      obras2.cumprimento_objeto_documentacao cod ON cex.cexid = cod.cexid
                    WHERE
                      cex.obrid = {$obrid}
                    ;";

        $extra = array();
        $resultado = $this->carregar($strSQL);

        if (!empty($resultado)) {
            $extra = $resultado;
        }

        return $extra;
    }
}
