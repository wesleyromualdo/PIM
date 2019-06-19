<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:24
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

class Planodecarreira extends Modelo {

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.planocarreiraprofessor";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pcpid');//,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pcpid' => null,
        'estuf' => null,
        'muncod' => null,
        'usucpf' => null,
        'docid' => null,
        'pcpstatus' => null,
        'docid2' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'pcpid',
        'estuf',
        'muncod',
        'usucpf',
        'docid',
        'pcpstatus',
        'docid2'
    );

    /**
     * Atributos da Tabela obrigatórios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'estuf',
        'muncod'
    );

    protected $atributosObrigatoriosString = array(
        'estuf' => 'Estado',
        'muncod' => 'Município'
    );

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'pcpid' => null,
        'docid' => null,
        'docid2'
    );

    public function retornaPerfil($usucpf = null){
        $arrPerfis = arrayPerfil($usucpf);

        if(!in_array(PFLCOD_SASE_SUPER_USUARIO, $arrPerfis)){
            if(in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $arrPerfis)){
                return PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE;
            } elseif(in_array(PFLCOD_SASE_TECNICO_DIVAPE, $arrPerfis)){
                return PFLCOD_SASE_TECNICO_DIVAPE;
            }
        } else {
            return PFLCOD_SASE_SUPER_USUARIO;
        }
    }

    public function retornaResponsabilidades($usucpf = null){
        global $db;
        $usucpf = $usucpf == null ? $_SESSION['usucpf'] : $usucpf;
        $arrPerfis = $this->retornaPerfil($usucpf);
        $ret = array(
            'estuf' => null,
            'muncod' => null
        );

        switch($arrPerfis){
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                $sql = "select estuf From sase.usuarioresponsabilidade where usucpf = '{$usucpf}' and rpustatus = 'A'";
                $ret['estuf'] = $db->carregarColuna($sql, 'estuf');
                break;

            case PFLCOD_SASE_TECNICO_DIVAPE:
                $sql = "select distinct mun.estuf From sase.usuarioresponsabilidade rpu inner join territorios.municipio mun on mun.muncod = rpu.muncod where usucpf = '{$usucpf}' and rpustatus = 'A'";
                $ret['estuf'] = $db->carregarColuna($sql, 'estuf');

                $sql = "select muncod from sase.usuarioresponsabilidade where usucpf = '{$usucpf}' and rpustatus = 'A'";
                $ret['muncod'] = $db->carregarColuna($sql, 'muncod');
                break;
        }

        return $ret;
    }

    public function getLista()
    {
        function formataPresencaArquivoPcr($campoArquivo)
        {
            if (empty($campoArquivo)) {
                return <<<HTML
<img src="/imagens/pnld/p_vermelho.gif" title="Sem PCR">
HTML;
            }
            return <<<HTML
<img src="/imagens/pnld/p_verde.gif" title="Com PCR">
HTML;
        }
        function formataPresencaArquivoAdesao($campoArquivo)
        {
            if (empty($campoArquivo)) {
                return <<<HTML
<img src="/imagens/pnld/p_vermelho.gif" title="Sem Termo de adesão">
HTML;
            }
            return <<<HTML
<img src="/imagens/pnld/p_verde.gif" title="Com Termo de adesão">
HTML;
        }
function formataPresencaArquivoAto($campoArquivo)
        {
            if (empty($campoArquivo)) {
                return <<<HTML
<img src="/imagens/pnld/p_vermelho.gif" title="Sem Ato Legal da Comissão">
HTML;
            }
            return <<<HTML
<img src="/imagens/pnld/p_verde.gif" title="Com Ato Legal da Comissão">
HTML;
        }
function formataPresencaArquivoMpl($campoArquivo)
        {
            if (empty($campoArquivo)) {
                return <<<HTML
<img src="/imagens/pnld/p_vermelho.gif" title="Sem Minuta do PL">
HTML;
            }
            return <<<HTML
<img src="/imagens/pnld/p_verde.gif" title="Com Minuta do PL">
HTML;
        }

        $pflcod = $this->retornaPerfil();
        $cols = "pcp.estuf, mun.mundescricao, spc.spcdsc, spm.spmdsc, pcp.pcppcr, pcp.pcpadesao, pcp.pcpato, pcp.pcpmpl";
        $where = " where 1=1 ";
        if($_POST['estuf']) { $where .= " and pcp.estuf  = '".$_POST['estuf']."'";  }
        if($_POST['muncod']){ $where .= " and pcp.muncod = '".$_POST['muncod']."'"; }
        if($_POST['esdid']) { $where .= " and doc.esdid  = ".$_POST['esdid'];  }
        if($_POST['esdid2']) { $where .= " and doc2.esdid  = ".$_POST['esdid2'];  }

        $where .= ('t' == $_REQUEST['pcppcr'])?' AND pcp.pcppcr IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcppcr'])?' AND pcp.pcppcr IS NULL':'';

        $where .= ('t' == $_REQUEST['pcpadesao'])?' AND pcp.pcpadesao IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcpadesao'])?' AND pcp.pcpadesao IS NULL':'';

        $where .= ('t' == $_REQUEST['pcpato'])?' AND pcp.pcpato IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcpato'])?' AND pcp.pcpato IS NULL':'';

        $where .= ('t' == $_REQUEST['pcpmpl'])?' AND pcp.pcpmpl IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcpmpl'])?' AND pcp.pcpmpl IS NULL':'';

        $join = "";
        switch($pflcod){
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                $join = "inner join sase.usuarioresponsabilidade rpu on rpu.estuf = mun.estuf and rpu.usucpf = '{$_SESSION['usucpf']}' and rpu.rpustatus = 'A' and rpu.pflcod = ".PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE;
                break;

            case PFLCOD_SASE_TECNICO_DIVAPE:
                $join = "inner join sase.usuarioresponsabilidade rpu on rpu.muncod = mun.muncod and rpu.usucpf = '{$_SESSION['usucpf']}' and rpu.rpustatus = 'A' and rpu.pflcod = ".PFLCOD_SASE_TECNICO_DIVAPE;
                break;
        }
        $sql = <<<DML
            select
                pcp.pcpid,
                {$cols}
            from sase.planocarreiraprofessor pcp
            inner join territorios.municipio mun on pcp.muncod = mun.muncod
            {$join}
            left join seguranca.usuario usu on usu.usucpf = pcp.usucpf
            left join workflow.documento doc on pcp.docid = doc.docid
            left join workflow.documento doc2 on pcp.docid2 = doc2.docid
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
            {$where}
            order by pcp.estuf, mun.mundescricao
DML;

        $list = new Simec_Listagem();
        $list->setQuery($sql)
            ->setCabecalho(array('Estado', 'Município', 'Situação Plano de Carreira', 'Situação Adequação', 'PCR', 'Termo adesão', 'Ato Legal da Comissão', 'Minuta PL'))
            ->addAcao('edit', array(
                'func' => 'editarPlanodeCarreira',
                'extra-params' => array(
                    'pcpid'
                )
            ))
            ->addCallbackDeCampo('pcppcr', 'formataPresencaArquivoPcr')
            ->addCallbackDeCampo('pcpadesao', 'formataPresencaArquivoAdesao')
            ->addCallbackDeCampo('pcpato', 'formataPresencaArquivoAto')
            ->addCallbackDeCampo('pcpmpl', 'formataPresencaArquivoMpl')
            ->setCampos(array('estuf', 'mundescricao', 'usunome'))
            ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
            ->setFormFiltros('formPesquisaLista')
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    //Legenda -->Principal/ Plano de Carreira de Profissionais de Ensino/ mapa  --> DIVAPE
    public function getLegenda(Array $estuf, $tpdid){
        global $db;
        $html = <<<HTML
            <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <ul>
HTML;
        if($estuf[0]){
            $where = $estuf != '' && count($estuf) > 0 ? " where pcp.estuf in ('".implode("','", $estuf)."') " : "";
        }
        
            $sql = "";
        switch ($tpdid){
            case '238':
                $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            {$where}
                            order by pcpid)
                    select
                        spc.spcid,
                        spc.spcdsc,
                        spc.spccor,
                        spc.esdid,
                        (select count(*) from temp_doc where spcid = spc.spcid) as total
                    from sase.sitplancarprofessor spc
                    group by spc.spcid, spc.spcdsc, spc.spccor
                    order by spc.spcid
DML;
                            
                break;
            case '240':
                $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid2 is null then (select spmid from sase.sitplanomunicipio where spmid = 1)
                                    else spm.spmid
                                end as spmid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid2 = doc.docid
                            left join sase.sitplanomunicipio spm on spm.esdid = doc.esdid
                            {$where}
                            order by pcpid)
                    select
                        spm.spmid as spcid,
                        spm.spmdsc as spcdsc,
                        spm.spccor,
                        spm.esdid,
                        (select count(*) from temp_doc where spmid = spm.spmid) as total
                    from sase.sitplanomunicipio spm
                    group by spm.spmid, spm.spmdsc, spm.spccor
                    order by spm.spmid
DML;

                break;
            case '242':
                $sql = <<<DML
                    SELECT spccor, spcdsc, count(spccor) as total
                    FROM (
                        SELECT 
                            CASE
                                WHEN pcppcr IS NOT NULL THEN '#006600'
                                WHEN pcppcr IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pcppcr IS NOT NULL THEN 'Com PCR'
                                WHEN pcppcr IS NULL THEN 'Sem PCR'
                            END as spcdsc
                        FROM sase.planocarreiraprofessor pcp
                        {$where}
                    ) t
                    GROUP BY spcdsc,spccor
DML;

                break;
            case '244':
                $sql = <<<DML
                    SELECT spccor, spcdsc, count(spccor) as total
                    FROM (
                        SELECT 
                            CASE
                                WHEN pcpadesao IS NOT NULL THEN '#006600'
                                WHEN pcpadesao IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pcpadesao IS NOT NULL THEN 'Com Termo de Adesão'
                                WHEN pcpadesao IS NULL THEN 'Sem Termo de Adesão'
                            END as spcdsc
                        FROM sase.planocarreiraprofessor pcp
                        {$where}
                    ) t
                    GROUP BY spcdsc,spccor
DML;

                break;
            case '246':
                $sql = <<<DML
                    SELECT spccor, spcdsc, count(spccor) as total
                    FROM (
                        SELECT 
                            CASE
                                WHEN pcpato IS NOT NULL THEN '#006600'
                                WHEN pcpato IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pcpato IS NOT NULL THEN 'Com Ato Legal da Comissão'
                                WHEN pcpato IS NULL THEN 'Sem Ato Legal da Comissão'
                            END as spcdsc
                        FROM sase.planocarreiraprofessor pcp
                        {$where}
                    ) t
                    GROUP BY spcdsc,spccor
DML;

                break;
        }
        $lis = $db->carregar($sql);
        foreach ($lis as $key => $value) {
            $total = $value['total'] != '' ? $value['total'] : 0;
            $html .= <<<HTML
                <li>
                    <table>
                        <tr>
                            <td>
                                <span style='background:{$value['spccor']};' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$total}</b>&nbsp;&nbsp;
                            </td>
                            <td>
                                {$value['spcdsc']}
                            </td>
                        </tr>
                    </table>
                </li>
HTML;
        }
        $html .= <<<HTML
            </ul>
        </div>
HTML;

        echo $html;
    }
    public function montaListaXls( $filtros = false ){
        global $db;

        $pflcod = $this->retornaPerfil();
        $cols = "pcp.estuf, mun.mundescricao, spc.spcdsc, spm.spmdsc, pcp.pcppcr, pcp.pcpadesao, pcp.pcpato, pcp.pcpmpl";
        $where = " where 1=1 ";
        if($_POST['estuf']) { $where .= " and pcp.estuf  = '".$_POST['estuf']."'";  }
        if($_POST['muncod']){ $where .= " and pcp.muncod = '".$_POST['muncod']."'"; }
        if($_POST['esdid']) { $where .= " and doc.esdid  = ".$_POST['esdid'];  }
        if($_POST['esdid2']) { $where .= " and doc2.esdid  = ".$_POST['esdid2'];  }

        $where .= ('t' == $_REQUEST['pcppcr'])?' AND pcp.pcppcr IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcppcr'])?' AND pcp.pcppcr IS NULL':'';

        $where .= ('t' == $_REQUEST['pcpadesao'])?' AND pcp.pcpadesao IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcpadesao'])?' AND pcp.pcpadesao IS NULL':'';

        $where .= ('t' == $_REQUEST['pcpato'])?' AND pcp.pcpato IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcpato'])?' AND pcp.pcpato IS NULL':'';

        $where .= ('t' == $_REQUEST['pcpmpl'])?' AND pcp.pcpmpl IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcpmpl'])?' AND pcp.pcpmpl IS NULL':'';

        $join = "";
        switch($pflcod){
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                $join = "inner join sase.usuarioresponsabilidade rpu on rpu.estuf = mun.estuf and rpu.usucpf = '{$_SESSION['usucpf']}' and rpu.rpustatus = 'A' and rpu.pflcod = ".PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE;
                break;

            case PFLCOD_SASE_TECNICO_DIVAPE:
                $join = "inner join sase.usuarioresponsabilidade rpu on rpu.muncod = mun.muncod and rpu.usucpf = '{$_SESSION['usucpf']}' and rpu.rpustatus = 'A' and rpu.pflcod = ".PFLCOD_SASE_TECNICO_DIVAPE;
                break;
        }

        $sql = "select
                --pcp.pcpid,
                {$cols}
            from sase.planocarreiraprofessor pcp
            inner join territorios.municipio mun on pcp.muncod = mun.muncod
            {$join}
            left join seguranca.usuario usu on usu.usucpf = pcp.usucpf
            left join workflow.documento doc on pcp.docid = doc.docid
            left join workflow.documento doc2 on pcp.docid2 = doc2.docid
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
            {$where}
            order by pcp.estuf, mun.mundescricao";

//        if( $filtros != false ){
//            $sql .= " WHERE 1=1 ";
//            foreach ($filtros as $chave => $valor)
//                $sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
//        }

        //$res = $db->carregar($sql);
//	ver($sql, d);
        $cabecalho = array('Estado', 'Município', 'Situação Plano de Carreira', 'Situação Adequação', 'PCR', 'Termo adesão', 'Ato Legal da Comissão', 'Minuta do PL');
        $alinhamento = array('left','left','left', "left",'left','left','left');
        $larguras = array('5%','5%','65%', '5%', '5%','5%','5%','5%');

        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=simec_sase_plano_de_carreira_" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=simec_sase_plano_de_carreira_" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");

        $db->monta_lista($sql,$cabecalho,1000000,5,'N','','N','listaPlanodeCarreira',$larguras,$alinhamento);
    }
    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
     *
     * @author Sávio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegendaMunicipioExterno($tipoinfo = Mapas::TIPO_ADEQUACAO)
    {
        global $db; ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <ul>
                <?php
                switch ($tipoinfo) {
                    case Mapas::PCR:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosPlanoCarreiraPCR.php';
                        $classeDados = 'DadosPlanoCarreiraPCR';
                        break;
                    case Mapas::ADESAO:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosPlanoCarreiraAdesao.php';
                        $classeDados = 'DadosPlanoCarreiraAdesao';
                        break;
                    case Mapas::ATO:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosPlanoCarreiraAto.php';
                        $classeDados = 'DadosPlanoCarreiraAto';
                        break;
                    case Mapas::PLANO_CARREIRA:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosPlanoCarreira.php';
                        $classeDados = 'DadosPlanoCarreira';
                        break;
                    case Mapas::ADEQUACAO:
                    default:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosPlanoCarreiraAdequacao.php';
                        $classeDados = 'DadosPlanoCarreiraAdequacao';
                }

                $legenda = (new $classeDados())->dadosDaLegenda();
				foreach ($legenda as $value) {
                echo <<<HTML
                <li>
                    <table>
                        <tr>
                            <td style="width:47px;">
                                <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$value['total']}</b>&nbsp;&nbsp;
                            </td>
                            <td>{$value['descricao']}</td>
                        </tr>
                    </table>
                </li>
HTML;
                }
                ?>
            </ul>
        </div>
    <?php
    }

}