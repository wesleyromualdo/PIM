<?php

class Diligencia extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.diligencia";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dlgid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array('dlgid'                      => null,
                                   'tpdid'                      => null,
                                   'fsdid'                      => null,
                                   'empid'                      => null,
                                   'usucpf'                     => null,
                                   'usucpfsuperacao'            => null,
                                   'dlgdsc'                     => null,
                                   'dlgdtprevisaoregularizacao' => null,
                                   'dlgdtsuperacao'             => null,
                                   'dlgdscprovidencia'          => null,
                                   'dlgsituacao'                => null,
                                   'dlgdtinclusao'              => null,
                                   'dlgstatus'                  => null,
                                   'obrid'                      => null,
                                   'dlgid_1'                    => null,
                                   'usucpfedicao'               => null,
                                   'docid'                      => null,
                                   'dlgflressalva'              => null,
                                   'dlgobsressalva'             => null
                                  );

    public function listaSql(Array $param = array()) {
        $arWhere = array();

        if (!empty($param['empid'])) {
            $param['empid'] = (array) $param['empid'];
            $arWhere[] = "d.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        if (!empty($param['obrid'])) {
            $param['obrid'] = (array) $param['obrid'];
            $arWhere[] = "d.obrid IN(" . implode(", ", $param['obrid']) . ")";
        }

        $perfilSuperUser = (possui_perfil(array(PFLCOD_SUPER_USUARIO)) ? 1 : 0);
        $perfilGestorMec = (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC)) ? 1 : 0);
        $perfilCallCenter = (possui_perfil(array(PFLCOD_CALL_CENTER)) ? 1 : 0);

        $param['block_imgs_acao'] = (possuiPerfil(array(PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR)) ? true : $param['block_imgs_acao']);

        if ($param['block_imgs_acao'] !== true || $perfilSuperUser || $perfilCallCenter) {
            $acao = "'<center><div style=\"width:50px;\">
                        <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarDelig(' || d.dlgid || ');\"
                            title=\"Alterar\">' ||
                        CASE
                            WHEN ({$perfilSuperUser} = 1) OR
                                 ({$perfilGestorMec} = 1) OR
                                 ( d.usucpfsuperacao IS NULL AND d.usucpf = '{$_SESSION['usucpf']}') THEN
                                '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir.gif\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    onclick=\"javascript: excluirDelig(' || d.dlgid || ');\"
                                    title=\"Excluir\">'
                            ELSE
                                '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                        END ||
                        CASE
                            WHEN ({$perfilSuperUser} = 1) AND ((SELECT 1 FROM obras2.historico_diligencia hd where hd.dlgid = d.dlgid LIMIT 1) = 1) THEN
                                '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/fluxodocm.gif\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    onclick=\"javascript: abreHistoricoDelig(' || d.dlgid || ');\"
                                    title=\"Diligncias Relacionadas\">'
                            ELSE
                                '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                        END ||'
                        </center></div>'";
        } else {
            $acao = "''";
        }

        $andamento = "CASE
                            WHEN esd.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . ")
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Correao\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Correo\">&nbsp;&nbsp;'
                                    END


                            WHEN esd.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . " )
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - dlgdtinclusao) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                    END


                            WHEN esd.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . ", " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_alerta.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'


                            WHEN esd.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_ativo.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Análise FNDE\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido_2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Correo\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_JUSTIFICADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido_2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Correo\">&nbsp;&nbsp;'
                        END
                         ";

        $analise = "
                        CASE
                            WHEN esd.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . ", " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE . "
                                 THEN
                                 CASE
                                        WHEN DATE_PART('days', NOW() - htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerata.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                    END

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_JUSTIFICADA. "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido_2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"justificada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_DILIGENCIA_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'
                        END
        ";

        $sql = "SELECT
                                {$acao} AS acao,
                                '<center>' || {$andamento} || '</center>' AS andamento, --Andamento
                                d.dlgid,
                                CASE WHEN d.fsdid IS NOT NULL THEN fd.fsddsc ELSE 'No Informada' END AS fase,
                                td.tpddsc,
                                TO_CHAR(d.dlgdtinclusao, 'DD/MM/YYYY') AS dlgdtinclusao,
                                d.dlgdsc,
                                d.dlgdscprovidencia,
                                TO_CHAR(d.dlgdtprevisaoregularizacao, 'DD/MM/YYYY') AS dlgdtprevisaoregularizacao,

                                CASE WHEN (SELECT
                                           MAX(p.pflcod)
                                           FROM
                                               seguranca.perfil p
                                           INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = usu.usucpf AND sisid = 147
                                            WHERE
                                              p.pflstatus='A'
                                            GROUP BY pu.usucpf) = " . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . "
                                THEN
                                    (SELECT DISTINCT
                                          e.entnome AS descricao
                                     FROM
                                          obras2.usuarioresponsabilidade ur
                                     INNER JOIN entidade.entidade e ON e.entid = ur.entid
                                     WHERE
                                        ur.usucpf = usu.usucpf AND
                                        ur.pflcod = " . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . " AND
                                        ur.rpustatus='A')
                                ELSE
                                    usu.usunome
                                END AS criadopor,


                                CASE WHEN d.dlgsituacao = TRUE THEN TO_CHAR(d.dlgdtsuperacao, 'DD/MM/YYYY') ELSE 'No' END AS dlgdtsuperacao,
                                esd.esddsc,
                                sup.usunome as ususuperacao,

                                CASE WHEN esd.esdid NOT IN (" . ESDID_DILIGENCIA_SUPERADA . ", " . ESDID_DILIGENCIA_JUSTIFICADA . ") THEN
                                    'NO'
                                WHEN esd.esdid IN (" . ESDID_DILIGENCIA_SUPERADA . ", " . ESDID_DILIGENCIA_JUSTIFICADA . ") AND d.dlgflressalva = 'S' THEN
                                    'SIM'
                                ELSE 'NO' END,

                                usuh.usunome as usuh,
                                TO_CHAR(h.htddata, 'DD/MM/YYYY HH:MM')
                        FROM
                                obras2.diligencia d
                        LEFT JOIN obras2.tipodiligencia       td ON td.tpdid = d.tpdid AND td.tpdstatus = 'A'
                        LEFT JOIN obras2.fasediligencia       fd ON fd.fsdid = d.fsdid AND fd.fsdstatus = 'A'
                        LEFT JOIN seguranca.usuario         usu ON usu.usucpf = d.usucpf
                        LEFT JOIN seguranca.usuario         sup ON sup.usucpf = d.usucpfsuperacao
                        LEFT JOIN workflow.documento        doc ON doc.docid  = d.docid
                        LEFT JOIN workflow.estadodocumento esd ON esd.esdid  = doc.esdid
                        LEFT JOIN workflow.historicodocumento h ON doc.hstid = h.hstid
                        LEFT JOIN seguranca.usuario         usuh ON usuh.usucpf = h.usucpf
                        WHERE
                                d.dlgstatus = 'A' AND " .
                ( implode(" AND ", $arWhere) ) . "
                        ORDER BY
                            d.dlgid";

        return $sql;
    }


    /**
     * Recebe o docid da Restrição/Inconformidade e retorna a linha com os dados da Última tramitao da Evoluo
     * @param int $docid
     * @return array $dados_tramitacao
     */
    public function getDadosTramitacaoDiligencia($docid = null, $campos = null, $where = null){

        if($docid == null){
            $docid = $this->docid;
            if($docid == null){
                return array();
            }
        }

        if(empty($campos)){
            $campos = "
                        dlg.obrid,
                        dlg.dlgid,
                        dlg.usucpf as usu_cpf_cad_dlg,
                        dlg.docid,
                        cmd.cmddsc,

                        usu_os.usunome as usu_nome_cad_dlg,
                        tpd.tpddsc,
                        doc.docdsc,
                        esd.esdid,
                        esd.esddsc,
                        hst.hstid,
                        hst.usucpf        as usu_cpf_exec_tramitacao,
                        usu_acao.usunome  as usu_nome_exec_tramitacao,
                        usu_acao.usuemail as usu_email_exec_tramitacao,
                        hst.htddata,
                        aed.aedid,
                        aed.aeddscrealizada,
                        aed.esdidorigem,
                        aed.esdiddestino,
                        esdorigem.esddsc  as estado_de_origem,
                        esddestino.esddsc as estado_de_destino,
                        COALESCE(to_char(hst.htddata, 'DD/MM/YYYY')) AS data_tramitacao,
                        COALESCE(to_char(hst.htddata, 'YYYY-MM-DD')) AS data_tramitacao2
                      ";
        }

        if(empty($where)){
            $where = '';
        }

        $sql = " SELECT
                        {$campos}

                 FROM obras2.diligencia dlg

                 LEFT JOIN workflow.documento           doc ON doc.docid  = dlg.docid
                 LEFT JOIN workflow.tipodocumento       tpd ON tpd.tpdid  = doc.tpdid
                 LEFT JOIN workflow.estadodocumento     esd ON esd.esdid  = doc.esdid
                 LEFT JOIN workflow.historicodocumento  hst ON hst.hstid  = doc.hstid
                 LEFT JOIN workflow.comentariodocumento cmd ON cmd.hstid  = hst.hstid
                 LEFT JOIN workflow.acaoestadodoc       aed ON aed.aedid  = hst.aedid
                 LEFT JOIN seguranca.usuario         usu_os ON dlg.usucpf = usu_os.usucpf
                 LEFT JOIN seguranca.usuario       usu_acao ON hst.usucpf = usu_acao.usucpf

                 LEFT JOIN workflow.estadodocumento esdorigem  ON aed.esdidorigem  = esdorigem.esdid
                 LEFT JOIN workflow.estadodocumento esddestino ON aed.esdiddestino = esddestino.esdid

                 WHERE doc.docid = ".$docid."
                   AND dlg.dlgstatus = 'A'
                   {$where}
                 ORDER BY
                    hst.htddata DESC
                ";

        $dados_tramitacao = $this->pegaLinha($sql);
        $dados_tramitacao = ($dados_tramitacao) ? $dados_tramitacao  : array();

        return $dados_tramitacao;
    }

    public function salvaDiligencia($param, $obrid = null) {

        foreach ($param as $key => $value) {
            if(!is_array($value)) {
                if($value != strip_tags($value)) {
                    $param[$key] = strip_tags($value, '<strong><p><br>');
                    $param[$key] = addslashes($param[$key]);
                }
            }
        }

        if(!empty($_REQUEST['dlgid'])){
            $param['dlgid'] = $_REQUEST['dlgid'];
        }

        if(!empty($param['dlgdtprevisaoregularizacao'])){
            $data = explode('/', $param['dlgdtprevisaoregularizacao']);
            $param['dlgdtprevisaoregularizacao'] = $data[2].'-'.$data[1].'-'.$data[0];
        }

        if($obrid){
            $obra = $obrid;
        }else{
            $obra = ($_GET['acao'] != 'A') ? $_SESSION['obras2']['obrid'] : null;
            if($obra == null){
                $obra = $obrid;
            }
        }

        $dados = array('dlgid'                      => ((empty($param['dlgid']) || trim($param['dlgid'] == '')) ? $_POST['dlg'] : $param['dlgid']),
            'tpdid'                      => ((empty($param['tpdid']) || trim($param['tpdid'] == '')) ? 12   : $param['tpdid']),
            'fsdid'                      => ((empty($param['fsdid']) || trim($param['fsdid'] == '')) ? null : $param['fsdid']),
            'empid'                      => ( ( ( $_GET['acao'] == 'A' ) ) ? $_SESSION['obras2']['empid'] : $param['empid'] ),
            'dlgdsc'                     => ((empty($param['dlgdsc']) || trim($param['dlgdsc'] == '')) ? '' : "'".  addslashes(strip_tags($param['dlgdsc'], '<strong><p><br>'))."'"),
            'dlgdtprevisaoregularizacao' => ((empty($param['dlgdtprevisaoregularizacao']) || trim($param['dlgdtprevisaoregularizacao'] == '')) ? null : "'".$param['dlgdtprevisaoregularizacao']."'"),
            'dlgdscprovidencia'          => ((empty($param['dlgdscprovidencia']) || trim($param['dlgdscprovidencia'] == '')) ? '' : "'".addslashes(strip_tags($param['dlgdscprovidencia'], '<strong><p><br>'))."'"),
            'obrid'                      => $obra,
            'dlgitem'                    => ((empty($param['dlgitem']) || trim($param['dlgitem'] == '')) ? null : "'".$param['dlgitem']."'"),
            'dlgflressalva'              => ((empty($param['dlgflressalva']) || trim($param['dlgflressalva'] == ''))   ? null : "'".$param['dlgflressalva']."'"),
            'dlgobsressalva'             => ((empty($param['dlgobsressalva']) || trim($param['dlgobsressalva'] == '')) ? '' : "'".addslashes(strip_tags($param['dlgobsressalva'], '<strong><p><br>'))."'" )
        );

        if(empty($param['dlgid'])){

            $dados['dlgdtinclusao'] = "now()";
            $dados['dlgstatus']     = "'A'";
            $dados['usucpf']        = "'".$_SESSION['usucpf']."'";

            foreach ($dados as $k => $v) {
                if($v == null){
                    unset($dados[$k]);
                }else{
                    $insert_campos  .= $k.",";
                    $insert_valores .= $v.",";
                }
            }
            $sql_insert = " INSERT INTO obras2.diligencia
                                 (".substr($insert_campos,0,-1).")
                               VALUES
                               (".substr($insert_valores,0,-1).")
                               RETURNING dlgid; ";
            try{
                $dlgid = $this->pegaUm($sql_insert);
            } catch (Exception $ex) {
                $this->rollback();
                $arr_return['erro'] = true;
                $arr_return['msg'] .= '\nErro ao cadastrar a Diligência . '.$ex->getMessage();
            }

        }
        else{
            $dados['usucpfedicao']        = "'".$_SESSION['usucpf']."'";

            $dlgid = $dados['dlgid'];
            $update_campos  = "";
            foreach ($dados as $key => $value) {
                if($value == null){
                    unset($dados[$key]);
                }else{
                    $update_campos  .= $key." = ".$value.',';
                }
            }
          $update = " UPDATE obras2.diligencia
                        SET ".substr($update_campos,0,-1)."
                        WHERE dlgid = ".$dlgid;

            try{
                $this->executar($update);
            } catch (Exception $ex) {
                $this->rollback();
                $arr_return['erro'] = true;
                $arr_return['msg'] .= '\nErro ao atualizar os dados da Diligência . Erro: '.$ex->getMessage();
            }

        }

        $this->carregarPorId($dlgid);
        return $dlgid;
    }

    /**
     * Método usado para atualizar as Restrições que foram cadastradas sem DOCID.
     * Tambm usado em outras partes do sistema alm do arquivo /simec/seguranca/www/scripts_exec/obras2_atualizaDocidRestricaoInconformidade.php
     *
     * @param type $rstid
     * @param type $ret_docid
     * @return boolean
     */
    public function atualizaDocidNullDiligencia($dlgid = null, $ret_docid = null){
        if(empty($dlgid)){ $dlgid = $this->dlgid; if(empty($dlgid)){ return false; } }

        $sql = 'SELECT dlg.docid FROM obras2.diligencia dlg WHERE dlg.dlgid = '.$dlgid;
        $docid_teste = $this->pegaUm($sql);
        if(!empty($docid_teste)){
            return $docid_teste;
        }

        $docid = $this->criarDocidDiligencia($dlgid);
        $resp  = $this->atualizaDocidDiligencia($docid, $dlgid);
        /**
         * Verificar superao e atualizar o docid para o estado de superado, caso necessrio.
         */
        if($this->verificaDiligenciaSuperada($dlgid) && $resp){
            $resp = $this->atualizaDocidDiligenciaSuperado($dlgid, $docid);
            $this->posAcoesWfDiligencia($dlgid);
        }

        if($ret_docid == null){
            return $resp;
        }elseif($ret_docid == null && $resp){
            return $docid;
        }else{
            return false;
        }
    }

    public function criarDocidDiligencia($dlgid){
        if (empty($dlgid)) {
            $dlgid = $this->dlgid;
            if(empty($dlgid)){
                return false;
            }
        }
        require_once APPRAIZ . 'includes/workflow.php';
        $docdsc = "Fluxo de Diligência do mdulo Obras II - dlgid: " . $dlgid;
        $docid = wf_cadastrarDocumento(TPDID_DILIGENCIA, $docdsc);
        return $docid;
    }

    public function atualizaDocidDiligencia($docid, $dlgid) {
        if(empty($docid) || empty($dlgid)){
            return false;
        }
        $sql = 'UPDATE obras2.diligencia SET docid = ' . $docid . ' WHERE dlgid = ' . $dlgid;
        try {
            $resp = $this->executar($sql);
            $this->commit();
        } catch (Exception $exc) {
            $resp = false;
        }
        return $resp;
    }

    public function verificaDiligenciaSuperada($dlgid){
        $sql = 'SELECT dlg.dlgsituacao FROM obras2.diligencia dlg WHERE dlg.dlgid = '.$dlgid;
        $flag_superada = ($this->pegaUm($sql) == 't') ? true : false ;
        return $flag_superada;
    }

    public function atualizaDocidDiligenciaSuperado($dlgid = null, $docid = null)
    {
        if (empty($dlgid)) {
            $dlgid = $this->dlgid;
            if (empty($dlgid)) {
                return false;
            }
        }

        if (empty($docid)) {
            $docid = $this->docid;
            if (empty($docid)) {
                return false;
            }
        }

        $aedid  = AEDID_DILIGENCIA_CONFIRMAR_SUPERACAO;
        $esdid  = ESDID_DILIGENCIA_SUPERADA;
        $cmddsc = 'Diligência superada de forma automatizada de acordo com os dados legado de antes da implementação do Workflow nas Restrições.';

        // cria log no histórico
        $sqlHistorico = "insert into workflow.historicodocumento(aedid, docid, usucpf, htddata)
                          values ({$aedid}, {$docid}, '{$_SESSION['usucpf']}', now())
                          returning hstid";

        $hstid = (integer) $this->pegaUm( $sqlHistorico );

        if (!$hstid){
            $this->rollback();
            return false;
        }

        // cria comentario para essa ao automatizada
        $sqlComentario = " insert into workflow.comentariodocumento
                           ( docid, hstid, cmddsc, cmddata, cmdstatus )
                           values ( " . $docid . ", " . $hstid . ", '" . addslashes($cmddsc) . "', now(), 'A' )";

        if (!$this->executar( $sqlComentario )){
            $this->rollback();
            return false;
        }

        // atualiza documento
        $sqlDocumento = " update workflow.documento
                          set esdid   = " . $esdid . "
                          where docid = " . $docid;

        if (!$this->executar($sqlDocumento)) {
            $this->rollback();
            return false;
        }
        return true;
    }


    public function posAcoesWfDiligencia($dlgid) {

        $this->carregarPorId( $dlgid );
        $arr_dados         = $this->getDados();
        $docid             = $arr_dados['docid'];
        $arr_dados_tramite = $this->getDadosTramitacaoDiligencia($docid, null, null);
        $acao              = $arr_dados_tramite['aedid'];

        switch ($acao) {
            case AEDID_DILIGENCIA_CANCELAR:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_CANCELAR;
                break;
            case AEDID_DILIGENCIA_ENCAMINHAR_PARA_ANALISE:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_ENCAMINHAR_PARA_ANALISE;
                break;
            case AEDID_DILIGENCIA_CONFIRMAR_SUPERACAO:
                //Muda a flag de superado para true
               if(!$this->verificaDiligenciaSuperada($dlgid)){
                    $this->superaDiligencia($dlgid);
                }
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_CONFIRMAR_SUPERACAO;
                return true;
                break;
            case AEDID_DEVOLVER_PARA_CORRECAO:
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_DEVOLVER_PARA_CORRECAO;

                // Verifica as soolicitaes de desembolso
                $s = new SolicitacaoDesembolso();
                $s->verificaTramiteSolicitacoes($this->obrid);

                break;
            case AEDID_DILIGENCIA_RETORNAR_PARA_ANALISE:
                //Muda a flag de superado para false
                $this->retornaAnaliseDiligencia($dlgid);
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_RETORNAR_PARA_ANALISE;
                break;
            case AEDID_DILIGENCIA_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA;
                break;
            case AEDID_ENVIAR_PARA_ANALISE_FNDE:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_DILIGENCIA_ENVIAR_PARA_ANALISE_FNDE;
                break;
            default:
                //Do nothing
                break;
        }

//        $this->enviaEmailTramiteDiligencia($dlgid, $arr_dados_tramite);
    }

    public function superaDiligencia($dlgid){

        $usucpfsuperacao = $_SESSION['usucpf'];
        $dlgdtsuperacao  = $this->getDataEstadoAnaliseFNDE($dlgid);
        $dlgsituacao     = 't';

       $sql = " UPDATE obras2.diligencia
                 SET usucpfsuperacao = '".$usucpfsuperacao."',
                     dlgdtsuperacao  = '".$dlgdtsuperacao."',
                     dlgsituacao     = '".$dlgsituacao."'
                 WHERE dlgid = ".$dlgid ;
        $resp = $this->executar($sql);
            if($resp) {
                $this->commit();
            }
        return $resp;

    }

    public function getDataEstadoAnaliseFNDE($dlgid) {
        $this->carregarPorId( $dlgid );
        $arr_dados = $this->getDados();
        $docid     = $arr_dados['docid'];
        $campos    = ' hst.htddata ';
        $where     = 'AND esddestino.esdid = '.ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE;
        $dados     = $this->getDadosTramitacaoDiligencia($docid, $campos, $where);
        $data      = (!empty($dados['htddata'])) ? $dados['htddata'] : date('Y-m-d');
        return $data;
    }



    public function retornaAnaliseDiligencia($dlgid){

        $usucpfsuperacao = 'NULL';
        $dlgdtsuperacao  = 'NULL';
        $dlgsituacao     = 'f';

       $sql = " UPDATE obras2.diligencia
                 SET usucpfsuperacao = ".$usucpfsuperacao.",
                     dlgdtsuperacao  = ".$dlgdtsuperacao.",
                     dlgsituacao     = '".$dlgsituacao."'
                 WHERE dlgid = ".$dlgid ;
        $resp = $this->executar($sql);
        if($resp) {
            $this->commit();
        }

        return $resp;
    }

    public function gravaArquivosDiligencia($dlgid, $post = array()){

        $erro     = false;
        $msg_erro = '';

        if(empty($post)){ $post = $_POST; }

        $arquivo = $_FILES['arquivo'];

        $qtd_arquivos = count($arquivo['name']);

        for($c=0;$c<$qtd_arquivos;$c++){

            $arquivosparts    = explode(".", $arquivo["name"][$c]);
            $arqnome          = current($arquivosparts);
            $arqextensao      = end($arquivosparts);
            $descricaoArquivo = $post['rardesc'][$c];

            $sql = "INSERT INTO public.arquivo (arqnome,
                                                arqextensao,
                                                arqdescricao,
                                                arqtipo,
                                                arqtamanho,
                                                arqdata,
                                                arqhora,
                                                usucpf,
                                                sisid,
                                                arqstatus)
                            VALUES( '".simec_addslashes($arqnome)."',
                                    '".$arqextensao."',
                                    '".$descricaoArquivo."',
                                    '".$arquivo["type"][$c]."',
                                    '".$arquivo["size"][$c]."',
                                    '".date('Y-m-d')."',
                                    '".date('H:i:s')."',
                                    '".$_SESSION["usucpf"]."',
                                     ".$_SESSION["sisid"].",
                                    'A')
                            RETURNING arqid;";
            try{
                $arqid = $this->pegaUm($sql);
            } catch (Exception $ex) {
                $arqid     = false;
                $erro      = true;
                $msg_erro .= 'Erro ao cadastrar o arquivo pblico.'.$ex->getMessage();
            }

            if($arqid){
                $nomeEsquema = 'obras2';
                $caminhoArquivo = APPRAIZ."arquivos/".$nomeEsquema."/".floor($arqid/1000).'/'.$arqid;

                if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema)) {
                    mkdir(APPRAIZ."arquivos/".$nomeEsquema, 0777);
                }
                if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000))) {
                    $pasta = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000);
                    $resp = mkdir($pasta, 0777, true);
                }

                switch($arquivo["type"][$c]) {
                    case 'image/jpeg':
                        ini_set("memory_limit", "128M");
                        list($width, $height) = getimagesize($arquivo["tmp_name"][$c]);
                        $original_x = $width;
                        $original_y = $height;
                        // se a largura for maior que altura
                        if($original_x > $original_y) {
                            if($original_x != 0){
                                $porcentagem = (100 * 640) / $original_x;
                            }else{
                                $porcentagem = 0;
                            }
                        }else {
                            if($original_y != 0){
                                $porcentagem = (100 * 480) / $original_y;
                            }else{
                                $porcentagem = 0;
                            }
                        }

                        $tamanho_x = $original_x * ($porcentagem / 100);
                        $tamanho_y = $original_y * ($porcentagem / 100);
                        $image_p   = imagecreatetruecolor($tamanho_x, $tamanho_y);
                        $image     = imagecreatefromjpeg($arquivo['tmp_name'][$c]);

                        imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                        imagejpeg($image_p, $caminhoArquivo, 100);
                        ImageDestroy($image_p);//Clean-up memory
                        ImageDestroy($image);//Clean-up memory

                        break;
                    default:
                        if ( !move_uploaded_file( $arquivo["tmp_name"][$c], $caminhoArquivo) ) {
                            $erro      = true;
                            $msg_erro .= ' \n Problemas no envio do arquivo '.$c.' da lista.';
                        }
                }

                // 21 - Outros
                $tpaid = (empty($post['tpaid'][$c])) ? 21 : $post['tpaid'][$c];

                $sql = "INSERT INTO obras2.diligencia_arquivo (dlgid,
                                                              arqid,
                                                              tpaid,
                                                              dardesc,
                                                              dardata,
                                                              darstatus,
                                                              dardtinclusao
                                                             )
                                                    VALUES( $dlgid,
                                                            $arqid,
                                                            ".$tpaid.",
                                                           '".$descricaoArquivo."',
                                                            now(),
                                                            'A',
                                                            now()
                                                          )
                                                    RETURNING darid;";

                try{
                    $darid = $this->pegaUm($sql);
                } catch (Exception $ex) {
                    $darid     = false;
                    $erro      = true;
                    $msg_erro .= 'Erro ao cadastrar o arquivo da diligência.'.$ex->getMessage();
                }
            }
        }

        if($erro){
            $this->rollback();
            $msg = $msg_erro;
        }else{
            $this->commit();
            $msg = ' Arquivos cadastrados com sucesso. ';
        }

        return array('erro'=>$erro, 'msg'=>$msg);

    }

    public function gravaArquivos($dlgid, $post = array()){

        $erro     = false;
        $msg_erro = '';

        if(empty($post)){ $post = $_POST; }

        $arquivo = $_FILES['arquivo'];

        $qtd_arquivos = count($arquivo['name']);

        for($c=0;$c<$qtd_arquivos;$c++){
            $arquivosparts    = explode(".", $arquivo["name"][$c]);

            $arqnome          = current($arquivosparts);
            $arqextensao      = end($arquivosparts);
            $descricaoArquivo = $post['rardesc'][$c];

            $sql = "INSERT INTO public.arquivo (arqnome,
                                                arqextensao,
                                                arqdescricao,
                                                arqtipo,
                                                arqtamanho,
                                                arqdata,
                                                arqhora,
                                                usucpf,
                                                sisid,
                                                arqstatus)
                            VALUES( '".simec_addslashes($arqnome)."',
                                    '".$arqextensao."',
                                    '".$descricaoArquivo."',
                                    '".$arquivo["type"][$c]."',
                                    '".$arquivo["size"][$c]."',
                                    '".date('Y-m-d')."',
                                    '".date('H:i:s')."',
                                    '".$_SESSION["usucpf"]."',
                                     ".$_SESSION["sisid"].",
                                    'A')
                            RETURNING arqid;";
            try{
                $arqid = $this->pegaUm($sql);

            } catch (Exception $ex) {
                $arqid     = false;
                $erro      = true;
                $msg_erro .= 'Erro ao cadastrar o arquivo pblico.'.$ex->getMessage();
            }
            $tpaid = (empty($post['tpaid'][$c])) ? 21 : $post['tpaid'][$c];

            $string .=  ''.$arqid.','.$post['tpaid'][$c].','.$post['rardesc'][$c].'&';

            if($arqid){
                $nomeEsquema = 'obras2';
                $caminhoArquivo = APPRAIZ."arquivos/".$nomeEsquema."/".floor($arqid/1000).'/'.$arqid;

                if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema)) {
                    mkdir(APPRAIZ."arquivos/".$nomeEsquema, 0777);
                }
                if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000))) {
                    $pasta = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000);
                    $resp = mkdir($pasta, 0777, true);
                }

                switch($arquivo["type"][$c]) {
                    case 'image/jpeg':
                        ini_set("memory_limit", "128M");
                        list($width, $height) = getimagesize($arquivo["tmp_name"][$c]);
                        $original_x = $width;
                        $original_y = $height;
                        // se a largura for maior que altura
                        if($original_x > $original_y) {
                            if($original_x != 0){
                                $porcentagem = (100 * 640) / $original_x;
                            }else{
                                $porcentagem = 0;
                            }
                        }else {
                            if($original_y != 0){
                                $porcentagem = (100 * 480) / $original_y;
                            }else{
                                $porcentagem = 0;
                            }
                        }

                        $tamanho_x = $original_x * ($porcentagem / 100);
                        $tamanho_y = $original_y * ($porcentagem / 100);
                        $image_p   = imagecreatetruecolor($tamanho_x, $tamanho_y);
                        $image     = imagecreatefromjpeg($arquivo['tmp_name'][$c]);

                        imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                        imagejpeg($image_p, $caminhoArquivo, 100);
                        ImageDestroy($image_p);//Clean-up memory
                        ImageDestroy($image);//Clean-up memory

                        break;
                    default:
                        if ( !move_uploaded_file( $arquivo["tmp_name"][$c], $caminhoArquivo) ) {
                            $erro      = true;
                            $msg_erro .= ' \n Problemas no envio do arquivo '.$c.' da lista.';
                        }
                }

            }
            if($erro){
                $this->rollback();
                $msg = $msg_erro;
            }else{
                $this->commit();
             }


        }
        return array('erro'=>$erro, 'msg'=>$msg, 'sql' => $string);
    }


    public function gravaArquivosDiligenciaLote($sql,$dlgid){
        $sql = substr($sql,0, $size-1);
        $dadosPergunta = explode("&",$sql);
        $i=0;
        foreach ($dadosPergunta as $dadosArr) {

            $dados = explode(",", $dadosArr);
            $arqid =  $dados[0];
            $tpaid = $dados[1];
            $descricaoArquivo = $dados[2];

           $sql = "INSERT INTO obras2.diligencia_arquivo (dlgid,
                                                              arqid,
                                                              tpaid,
                                                              dardesc,
                                                              dardata,
                                                              darstatus,
                                                              dardtinclusao
                                                             )
                                                    VALUES( $dlgid,
                                                            $arqid,
                                                            ".$tpaid.",
                                                           '".$descricaoArquivo."',
                                                            now(),
                                                            'A',
                                                            now()
                                                          )
                                                    RETURNING darid;";



        try{
            $darid = $this->pegaUm($sql);
        } catch (Exception $ex) {
            $darid     = false;
            $erro      = true;
            $msg_erro .= 'Erro ao cadastrar o arquivo da diligência.'.$ex->getMessage();
        }
            $i++;

        }
        return array('erro'=>$erro, 'msg'=>$msg);
    }



    public function getListaArquivosDiligencia($dlgid) {

        if(empty($dlgid)){ return array(); }

        $diligencia = new Diligencia($dlgid);
        $esdid = wf_pegarEstadoAtual($diligencia->docid);

        if(possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC ))){
            $img_excluir = "<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir_2.gif\"
                                    width=\"19px\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    id=\"' || da.arqid || '_' || da.dlgid || '\"
                                    class=\"excluir\"
                                    title=\"Excluir Arquivo\">
                    </center>";
        }else{
            $img_excluir = "</center>";
        }

        if(possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE)) && $esdid['esdid'] != ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA){
            $img_excluir = "</center>";
        }

        $sql = "SELECT
                        '<center>
                        <img
                            align=\"absmiddle\"
                            src=\"/imagens/icone_lupa.png\"
                            style=\"cursor: pointer\"
                            id=\"' || da.arqid || '_' || da.dlgid || '\"
                            class=\"download\"
                            title=\"Baixar Arquivo\">
                            $img_excluir
                        ' as acao,
                        ta.tpadesc,
                        COALESCE(da.dardesc, '')  as descricao,
                        '<a href=\"javascript:void(0);\"
                                id=\"' || da.arqid || '_' || da.dlgid || '\"
                                class=\"download\"
                                title=\"Baixar Arquivo\">'
                                || a.arqnome || '.' || a.arqextensao ||
                        '</a>' as nome_arquivo,
                        to_char(dardtinclusao,'DD/MM/YYYY') AS data,
                        usu.usunome
                FROM
                        obras2.diligencia_arquivo da
                JOIN obras2.tipoarquivo ta ON ta.tpaid   = da.tpaid
                JOIN public.arquivo      a ON a.arqid    = da.arqid
                JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
                WHERE
                      da.dlgid = ".$dlgid."
                  AND da.darstatus = 'A'
                ORDER BY
                        2";

        try{
            $dados = $this->carregar($sql);
        } catch (Exception $ex) {
            $dados = array();
        }

        $dados = (empty($dados)) ? array() : $dados ;

        return $dados;

    }

//    public function carregaDadosPorObrid($obrid) {
//        $sql = "SELECT
//                  u.usucpf AS usucpf,
//                  u.usunome AS usunome,
//                  usup.usucpf AS usucpfsuperacao,
//                  usup.usunome AS usunomesuperacao,
//                  CASE
//                      WHEN r.fsrid IS NOT NULL THEN
//                          fsrdsc
//                      ELSE
//                          'No Informada'
//                  END AS fsrdsc,
//                  tr.tprdsc,
//                  r.rstid,
//                  r.rstdsc,
//                  r.rstdscprovidencia,
//                  TO_CHAR(r.rstdtinclusao,'DD/MM/YYYY') AS rstdtinclusao,
//                  TO_CHAR(r.rstdtprevisaoregularizacao,'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
//                  CASE
//                      WHEN r.rstsituacao = true THEN
//                          to_char(r.rstdtsuperacao,'DD/MM/YYYY')
//                      ELSE 'No'
//                  END AS rstdtsuperacao
//              FROM
//                  obras2.restricao r
//              JOIN obras2.tiporestricao tr ON tr.tprid = r.tprid
//              LEFT JOIN obras2.faserestricao fr ON fr.fsrid = r.fsrid
//              JOIN seguranca.usuario u ON u.usucpf = r.usucpf
//              LEFT JOIN seguranca.usuario usup ON usup.usucpf = r.usucpfsuperacao
//              WHERE
//                  r.obrid = {$obrid}";
//        $dados = $this->carregar($sql);
//
//        return ($dados ? $dados : array());
//    }

    public function excluirArquivosDiligencia($arqid,$dlgid) {
        $erro = false;

        $sql = "UPDATE obras2.diligencia_arquivo SET darstatus = 'I' WHERE arqid = ".$arqid." AND dlgid = ".$dlgid;
        try{
            $resp = $this->executar($sql);
        } catch (Exception $ex) {
            $erro = true;
        }

        $sql1 = "SELECT count(darid) FROM obras2.diligencia_arquivo  WHERE  arqid = ".$arqid." AND darstatus = 'A'";
        try{
            $resp1 = $this->pegaUm($sql1);;
        } catch (Exception $ex) {
            $erro = true;
        }

        if($resp1 == 0 ){
            $sql2 = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid;
            try{
                $resp2 = $this->executar($sql2);
            } catch (Exception $ex) {
                $erro = true;
            }

         }

        if($erro){
            $this->rollback();
        }else{
            $this->commit();
        }

        return $resp;
    }



    public function getDadosListaDiligencia($tipo_retorno = 'sql', $tipo = 'lista') {

        $dados = array();

        $where = $this->getFiltrosDadosListaDiligencia();

        if($_GET['acao'] == 'L')
            $where[] = "obr.obrstatus = 'A'";

        $strWhere = ( (!empty($where)) ? ' AND ' . implode(' AND ', $where) : '');

        if (!possui_perfil(Array(PFLCOD_SUPER_USUARIO))) {

            $arrObr = Array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR/* ,PFLCOD_SUPERVISOR_UNIDADE */);
            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);
            $arrOrg = Array(PFLCOD_ADMINISTRADOR, PFLCOD_CADASTRADOR_INSTITUCIONAL, /*PFLCOD_CONSULTA_ESTADUAL, */ PFLCOD_CONSULTA_TIPO_DE_ENSINO, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC);
            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();

            if (possui_perfil($arrEst)) {
                $resp[]   = "urs.estuf = ende.estuf ";
                $arPflcod = array_merge($arPflcod, $arrEst);
            }

            if (possui_perfil($arrObr)) {
                $resp[]   = "urs.empid = ep.empid ";
                $arPflcod = array_merge($arPflcod, $arrObr);
            }

            if (possui_perfil($arrOrg)) {
                $resp[]   = "urs.orgid = ep.orgid ";
                $arPflcod = array_merge($arPflcod, $arrOrg);
            }

            if (possui_perfil($arrUni)) {
                $resp[]   = "urs.entid = ep.entidunidade ";
                $arPflcod = array_merge($arPflcod, $arrUni);
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid     = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid     = ($arEmpid ? $arEmpid : array(0));
                $resp[]      = " urs.entid = ep.entidunidade AND ep.empid IN('" . implode("', '", $arEmpid) . "')";
                $arPflcod[]  = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[]  = PFLCOD_CONSULTA_UNIDADE;
            }

            if (count($resp)) {
                $arPflcod = array_unique($arPflcod);
                $join['usuarioresponsabilidade'] = "JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    (" . implode(' OR ', $resp) . ")";
            }
        }else{
            $join['usuarioresponsabilidade'] = '';
        }

        $acao = "'<center><div style=\"width:50px;\">
                            <img
                                    align=\"absmiddle\"
                                    src=\"/imagens/alterar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: alterarDiligencia(' || dlg.dlgid || ' , '|| obr.obrid ||' , '|| obr.empid ||' );\"
                                    title=\"Alterar\">

                                    <a href=\"/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=' || obr.obrid || '\" target=\"_blank\"><img
                                    align=\"absmiddle\"
                                    src=\"/imagens/consultar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: abrirObra(' || obr.obrid || ' );\"
                                    title=\"Ver Obra\"></a>
                     </center></div>'  as acao,";

        $andamento = "CASE
                            WHEN doc_ri.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . ")
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW()  - (select htddata from workflow.historicodocumento where docid = doc_ri.docid  order by htddata desc limit 01)) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Correao\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Correo\">&nbsp;&nbsp;'
                                    END


                            WHEN doc_ri.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . " )
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - dlgdtinclusao) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                    END


                            WHEN doc_ri.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . ", " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_alerta.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'


                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_ativo.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Análise FNDE\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido_2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Correo\">&nbsp;&nbsp;'


                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_JUSTIFICADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido_2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Correo\">&nbsp;&nbsp;'

                        END as andamento,

                         ";

        $analise = "
                        CASE
                            WHEN doc_ri.esdid IN (" . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . ", " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE . "
                                 THEN
                                 CASE
                                        WHEN DATE_PART('days', NOW() - h.htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                    END

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_JUSTIFICADA. "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido_2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"justificada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_DILIGENCIA_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'
                        END as analise,
        ";

        $colunasXls = '';
        $tipologia = '';
        if($tipo == 'xls'){
            $tipologia = "tpo.tpodsc,";
            $acao = '';
            $andamento = '';
            $analise = '';
            $colunasXls = ",
                            SUBSTRING( regexp_replace(regexp_replace(substr(dlg.dlgdsc, 0, 1000), E'<.*?>', '', 'g' ), E'&nbsp;', '', 'g') FOR 1000) as rstdsc,
                            SUBSTRING(regexp_replace(regexp_replace(substr(dlg.dlgdscprovidencia, 0, 1000), E'<.*?>', '', 'g' ), E'&nbsp;', '', 'g') FOR 1000) as rstdscprovidencia,
                            (SELECT ARRAY_TO_STRING(ARRAY(
                                    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') || ';' || a.aeddscrealizada || ';' || u.usunome FROM workflow.documento d
                                    JOIN workflow.historicodocumento h ON h.docid = d.docid
                                    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                                    JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                                    WHERE d.docid = doc_ri.docid
                                    ORDER BY h.htddata ASC
                            ), ';')) as historico

            ";
        }

        //string que monta a concatenao do campo obrid + idsecretaria para ser montada na query
        $campoObridSec = montaCampoObridSec('obr');

        $sql = " SELECT {$acao}
                        {$campoObridSec} as obrid,
                        dlg.dlgid,
                        {$andamento}

                        CASE
                            WHEN dlg.docid IS NULL
                            THEN 'Diligncias sem Documento cadastrado'
                            ELSE esd_ri.esddsc
                        END as situacao, -- Situação

                        mun.estuf as estado, -- * Estado
                        mun.mundescricao as municipio, -- * MUNICIPIO,
                        CASE
                            WHEN ep.empesfera = 'M' THEN 'Municipal'
                            WHEN ep.empesfera = 'E' THEN 'Estadual'
                            WHEN ep.empesfera = 'F' THEN 'Federal'
                            ELSE ''
                        END as esfera,
                        CASE WHEN dlg.fsdid IS NOT NULL THEN fd.fsddsc ELSE 'No Informada' END AS fase,
                        td.tpddsc as tipo, -- * Tipo
                        $tipologia
                        '(' || {$campoObridSec} || ') ' || obr.obrnome  as nome_obra, -- * Nome da Obra

                        esd_obr.esddsc,

                        TO_CHAR(dlg.dlgdtinclusao, 'DD/MM/YYYY') as data_cadastro, -- * Data Cadastro
                        usu.usunome as usucriacao, -- * Criado por
                        TO_CHAR(dlg.dlgdtprevisaoregularizacao, 'DD/MM/YYYY') AS previsao_providencia_dt, -- * Previso da Providência
                        CASE WHEN dlg.dlgsituacao = TRUE THEN TO_CHAR(dlg.dlgdtsuperacao, 'DD/MM/YYYY') ELSE 'No' END AS superacao, -- * Superao
                        sup.usunome as ususuperacao, -- * Superado por
                        (
                            select usuh.usunome from workflow.historicodocumento dd
                            LEFT JOIN seguranca.usuario        usuh ON usuh.usucpf   = dd.usucpf
                            where dd.docid = doc_ri.docid
                            order by htddata desc limit 01
                        ),
                                    TO_CHAR( (
                            select htddata from workflow.historicodocumento dd
                            where dd.docid = doc_ri.docid
                            order by htddata desc limit 01
                        ), 'DD/MM/YYYY') as htddata
                        {$colunasXls}




                     FROM obras2.diligencia dlg

                     LEFT JOIN obras2.obras             obr ON obr.obrid  = dlg.obrid AND obr.obridpai IS NULL AND obr.obrstatus IN ('P', 'A')
                     INNER JOIN obras2.empreendimento    ep ON ep.empid   = obr.empid

                     LEFT JOIN workflow.documento        doc_ri ON doc_ri.docid  = dlg.docid
                     LEFT JOIN workflow.estadodocumento  esd_ri ON esd_ri.esdid  = doc_ri.esdid
                     LEFT JOIN workflow.documento       doc_obr ON doc_obr.docid = obr.docid
                     LEFT JOIN workflow.estadodocumento esd_obr ON esd_obr.esdid = doc_obr.esdid

                     LEFT JOIN obras2.programafonte          pf ON pf.prfid   = ep.prfid
                     LEFT JOIN obras2.tipoorigemobra        too ON too.tooid  = obr.tooid
                     LEFT JOIN obras2.tipodiligencia          td ON td.tpdid   = dlg.tpdid   AND td.tpdstatus   = 'A'
                     LEFT JOIN obras2.fasediligencia          fd ON fd.fsdid   = dlg.fsdid   AND fd.fsdstatus   = 'A'
                     LEFT JOIN obras2.tipologiaobra         tpo ON tpo.tpoid  = obr.tpoid
                     LEFT JOIN entidade.endereco           ende ON ende.endid = obr.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                     LEFT JOIN territorios.municipio        mun ON mun.muncod = ende.muncod
                     LEFT JOIN territorios.estado            uf ON mun.estuf  = uf.estuf
                     LEFT JOIN seguranca.usuario            usu ON usu.usucpf = dlg.usucpf
                     LEFT JOIN seguranca.usuario            sup ON sup.usucpf = dlg.usucpfsuperacao
                     /*LEFT JOIN workflow.historicodocumento h ON doc_ri.hstid  = h.hstid
                     LEFT JOIN seguranca.usuario        usuh ON usuh.usucpf   = h.usucpf*/
                     ".$join['usuarioresponsabilidade']."
                     WHERE dlg.dlgstatus = 'A'
                     " . $strWhere . "

                     ORDER BY dlg.dlgid, obr.obrid

                     ";

        if($tipo_retorno == 'sql'){
            return $sql;
        }else{
            $dados = $this->carregar($sql);
            $dados = (!empty($dados)) ? $dados : array();

            return $dados;
        }

    }


    public function getFiltrosDadosListaDiligencia() {
        $where = array();

        extract($_REQUEST);

        if (!empty($obrbuscatexto)) {
            $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrbuscatexto))));
            $where[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $obrbuscatexto . "%') ) ";
        }

        if (!empty($rstid)) {
            $where[] = " dlg.dlgid::text = '$rstid' ";
        }

        if (!empty($tpoid)) {

            if(is_array($tpoid) ){
                if($tpoid[0] != ''){
                    $tpoid = implode(', ', $tpoid);
                    $where[] = " obr.tpoid IN ($tpoid) ";
                }
            } else {
                $where[] = " obr.tpoid = $tpoid ";
            }
        }

//        if (!empty($empesfera)) {
            $where[] = " ep.empesfera = 'M' ";
//        }

//        if (!empty($estuf)) {
//            if (is_array($estuf)) {
//                foreach ($estuf as $k => $v) {
//                    $estuf[$k] = "'{$v}'";
//                }
//            } else {
//                $estuf = array("'{$estuf}'");
//            }
//            foreach ($estuf as $k => $v) {
//                if (trim($v) == '' || $v == "''" || empty($v)) {
//                    unset($estuf[$k]);
//                }
//            }
//            if (!empty($estuf)) {
//                $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
//            }
//        }

        $where[] = "mun.estuf IN ('SP')";

        if (!empty($muncod)) {
            if (is_array($muncod)) {
                foreach ($muncod as $k => $v) {
                    $muncod[$k] = "'{$v}'";
                }
            } else {
                $muncod = array("'{$muncod}'");
            }
            foreach ($muncod as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($muncod[$k]);
                }
            }
            if (!empty($muncod)) {
                $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
            }
        }

        if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
            $where[] = " esd_obr.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
        }
        if (!empty($tpdid)) {
            $where[] = " td.tpdid IN ( " . implode(",", $tpdid) . " ) ";
        }

        if (!empty($esdid_ri) && is_array($esdid_ri) && $esdid_ri[0] != '') {
            $where[] = " esd_ri.esdid IN ( " . implode(",", $esdid_ri) . " ) ";
        }

        if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
            $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
        }

        if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
            $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
        }

        if(!empty($dlgdtinclusao_de) && !empty($dlgdtinclusao_ate)){
            $de  = explode('/', $dlgdtinclusao_de);
            $ate = explode('/', $dlgdtinclusao_ate);
            $dlgdtinclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $dlgdtinclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " dlg.dlgdtinclusao BETWEEN '{$dlgdtinclusao_de}' AND '{$dlgdtinclusao_ate}' ";
        }elseif(!empty($dlgdtinclusao_de) && empty($dlgdtinclusao_ate)){
            $de  = explode('/', $dlgdtinclusao_de);
            $dlgdtinclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " dlg.dlgdtinclusao = '{$dlgdtinclusao_de}' ";
        }elseif(empty($dlgdtinclusao_de) && !empty($dlgdtinclusao_ate)){
            $ate = explode('/', $dlgdtinclusao_ate);
            $dlgdtinclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " dlg.dlgdtinclusao = '{$dlgdtinclusao_ate}' ";
        }

        if(!empty($dlgdtsuperacao_de) && !empty($dlgdtsuperacao_ate)){
            $de  = explode('/', $dlgdtsuperacao_de);
            $ate = explode('/', $dlgdtsuperacao_ate);
            $dlgdtsuperacao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $dlgdtsuperacao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " dlg.dlgdtsuperacao BETWEEN '{$dlgdtsuperacao_de}' AND '{$dlgdtsuperacao_ate}' ";
        }elseif(!empty($dlgdtsuperacao_de) && empty($dlgdtsuperacao_ate)){
            $de  = explode('/', $dlgdtsuperacao_de);
            $dlgdtsuperacao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " dlg.dlgdtsuperacao = '{$dlgdtsuperacao_de}' ";
        }elseif(empty($dlgdtsuperacao_de) && !empty($dlgdtsuperacao_ate)){
            $ate = explode('/', $dlgdtsuperacao_ate);
            $dlgdtsuperacao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " dlg.dlgdtsuperacao = '{$dlgdtsuperacao_ate}' ";
        }

        if(!empty($ultimotramite_de) && !empty($ultimotramite_ate)){
            $de  = explode('/', $ultimotramite_de);
            $ate = explode('/', $ultimotramite_ate);
            $ultimotramite_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $ultimotramite_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " (select htddata from workflow.historicodocumento where docid = doc_ri.docid  order by htddata desc limit 01) BETWEEN '{$ultimotramite_de}' AND '{$ultimotramite_ate}' ";
        }elseif(!empty($ultimotramite_de) && empty($ultimotramite_ate)){
            $de  = explode('/', $ultimotramite_de);
            $ultimotramite_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " (select htddata from workflow.historicodocumento where docid = doc_ri.docid  order by htddata desc limit 01) >= '{$ultimotramite_de}'::date ";
        }elseif(empty($ultimotramite_de) && !empty($ultimotramite_ate)){
            $ate = explode('/', $ultimotramite_ate);
            $ultimotramite_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " (select htddata from workflow.historicodocumento where docid = doc_ri.docid  order by htddata desc limit 01) >= '{$ultimotramite_ate}'::date ";
        }

        if ( !empty($dlgflressalva)){
            if($dlgflressalva == 'T'){
                $where[] = " (dlg.dlgflressalva IN('S','N') OR dlg.dlgflressalva IS NULL) ";
            }else{
                $where[] = " dlg.dlgflressalva = '".$dlgflressalva."' ";
            }
        }

        return $where;
    }

    public function condicaoSuperacaoDiligencia($dlgid) {
        $dados = $this->verificaRessalvaDiligencia($dlgid);
        if(empty($dados['dlgflressalva'])){
            return false;
        }
        return true;
    }

    public function verificaRessalvaDiligencia($dlgid, $tipo_retorno = 'bool') {
        if($dlgid == null){ $dlgid = $this->dlgid; if($this->dlgid == null){ return array(); } }
        $sql   = 'SELECT dlg.dlgflressalva, dlg.dlgobsressalva FROM obras2.diligencia dlg WHERE dlg.dlgid = '.$dlgid;
        $dados = $this->pegaLinha($sql);
        $dados = (empty($dados) ? array() : $dados);
        return $dados;
    }
}
