<?php

class TermoCompromissoPac extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.termocompromissopac";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("terid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'terid' => null,
        'terdatainclusao' => null,
        'usucpf' => null,
        'arqid' => null,
        'muncod' => null,
        'terstatus' => null,
        'arqidanexo' => null,
        'terassinado' => null,
        'proid' => null,
        'estuf' => null,
        'usucpfassinatura' => null,
        'terdataassinatura' => null,
        'terdocumento' => null,
        'terreformulacao' => null,
        'teridpai' => null,
        'titid' => null,
        'tpdcod' => null,
        'ternumero' => null,
        'terano' => null,
        'teracaoorigem' => null,
        'terdatainicio' => null,
        'terdatafim' => null,
        'terdatafimvigencia' => null,
        'terexoficio' => null
    );

    public function recuperaTermoPorMunicipio($muncod) {
        $sql = "select terid, arqid, to_char(terdatainclusao, 'YYYY') as teranoinclusao from {$this->stNomeTabela}  where muncod = '$muncod' and terstatus = 'A'";
        return $this->carregar($sql);
    }

    /**
     * Atualizar
     * @param type $arrCampo
     * @param type $terid
     * @return type
     */
    public function editar($arrCampo, $terid) {

        $strSet = "";
        foreach ($arrCampo as $key => $value) {
            $strSet .= "{$key}={$value},";
        }
        $strSet = substr($strSet, 0, -1);
        $sql = "
                UPDATE {$this->stNomeTabela}
                   SET {$strSet} WHERE terid = {$terid}";
        return $this->executar($sql);
    }

    public function dataFinalVigencia($proid) {
        $dtFimVigencia = '';
        if ($proid) {
            $dtFimVigencia = $this->pegaUm("SELECT to_char((select tcp.terdatafimvigencia from par.termocompromissopac tcp where tcp.proid = {$proid} AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY')");

            if(!$dtFimVigencia || $dtFimVigencia == ''){
                $dtFimVigencia = $this->pegaUm("SELECT par.retornavigenciaobrapac({$proid})");
            }
        }
        return $dtFimVigencia;
    }

    /**
     * Gerar Termo Ex-Oficio do PAC
     * @param type $terid
     * @param type $terDataFimEx
     * @return string $modeloExOficio
     */
    public function gerarTermoEx($terid, $terDataFimEx, $terJustificativa, $strAcao = 'EX') {
        try {
            $terDataFim = formata_data_sql($terDataFimEx);

            # 0 - Buscar Informacoes do Termo Atual
            $this->carregarPorId($terid);

            # 1 - Verifica se existe Solicitacao de Termo
            $solProrrogacaoTermo = new SolicitacaoProrrogacaoPrazoObra();
            $sppid = reset($solProrrogacaoTermo->recuperarTodos('sppid', array("terid = {$terid}", "proid = {$this->proid}")));
            $sppid = $sppid['sppid'] ? $sppid['sppid'] : null;

            # 2 - Insere o Registro no historico
            $arrData = array(
                'proid' =>  $this->proid,
                'usucpf' =>  $_SESSION['usucpf'],
                'sppid' => $sppid,
                'hvttipodeacao' => $strAcao,
                'hvtvigenciaanterior' => $this->terdatafim,
                'hvtvigencianova' => $terDataFim,
                'hvtdataacao' => 'NOW()',
                'hvtjustificativa' => $terJustificativa,
            );

            (new HistoricoVigenciaTermoPac())->insert($arrData);
            
            # 2 - Inativar Termo Atual
            $this->terdatafim = $terDataFim;
            $this->terdatafimvigencia = $terDataFim;
            $this->salvar();

            return $this->commit() ? $this->terid : null ;

        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
        }
    }

    public function gerarTermoRescisao($post) {

        global $db;
        extract($post);
        try {
            $terDataFim = formata_data_sql($trpdatafim);

            # 0 - Buscar Informacoes do Termo Atual
            $this->carregarPorId($terid);


            $sql = "SELECT mdoconteudo FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$mdoid;
            $mdoconteudo = $db->pegaUm($sql);

            $this->terid = '';
            $this->proid = $proid;
            $this->terdocumento = $mdoconteudo;
            $this->tpdcod = $tpdcod;
            $this->proid = $proid;
            $this->terdatafimvigencia = $terDataFim;
            $this->teridpai = $terid;
            $this->teracaoorigem = 'RESCISÃO';
            $this->usucpf = $_SESSION['usucpf'];
            $this->terdataassinatura =  'NOW()';
            $this->terdatainclusao =  'NOW()';
            $this->terassinado =  't';
            $this->usucpfassinatura =  null;


            $solProrrogacaoTermo = new SolicitacaoProrrogacaoPrazoObra();
            $sppid = reset($solProrrogacaoTermo->recuperarTodos('sppid', array("terid = {$terid}", "proid = {$proid}")));
            $sppid = $sppid['sppid'] ? $sppid['sppid'] : null;

            # 2 - Insere o Registro no historico
            $arrData = array(
                'proid' =>  $this->proid,
                'usucpf' =>  $_SESSION['usucpf'],
                'sppid' => $sppid,
                'hvttipodeacao' => 'RC',
                'hvtvigenciaanterior' => $terDataFim,
                'hvtvigencianova' => $terDataFim,
                'hvtdataacao' => 'NOW()',
                'hvtjustificativa' => $htrjustificativa,
            );


            (new HistoricoVigenciaTermoPac())->insert($arrData);

            # 2 - Inativar Termo Atual
            $this->terdatafim = $terDataFim;
            $this->terdatafimvigencia = $terDataFim;
            $this->salvar();

            return $this->commit() ? $this->terid : null ;




        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
        }
    }


    public function retornaPcSemValorAComprovarObras($id = ''){
        
        $result = '';
        if($id != '') {
            $sql = "SELECT tc.terid
                        FROM
                          par.termocompromissopac tc
                        JOIN par.processoobra pp ON tc.proid = pp.proid
                        JOIN obras2.situacaoprestacaocontasobras sp ON pp.proid = sp.proid_pac
                        JOIN workflow.documento d ON sp.docid = d.docid
                        WHERE tc.terid = {$id} AND tc.terstatus = 'A' AND d.esdid = ".ESDID_SPC_OBR_SEM_VLR_A_COMPROVAR;
            $result =  $this->pegaUm($sql);
        }
        return $result;
    }
}
