<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:24
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

class PlanodecarreiraEstado extends Modelo {

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.planocarreiraprofessorestado";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pceid');//,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pceid' => null,
        'estuf' => null,
        'usucpf' => null,
        'docid' => null,
        'pcestatus' => null,
        'docid2' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'pceid',
        'estuf',
        'usucpf',
        'docid',
        'pcestatus',
        'docid2'
    );

    /**
     * Atributos da Tabela obrigat?rios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'estuf'
    );

    protected $atributosObrigatoriosString = array(
        'estuf' => 'Estado'
    );

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'pceid' => null,
        'docid' => null,
        'docid2' => null
    );

    public function getLista(){
        function formataPresencaArquivoPcr($campoArquivo){
            if (empty($campoArquivo)) {
                return <<<HTML
                        <img src="/imagens/pnld/p_vermelho.gif" title="Sem PCR">
HTML;
            }
            return <<<HTML
                        <img src="/imagens/pnld/p_verde.gif" title="Com PCR">
HTML;
        }
        function formataPresencaArquivoAdesao($campoArquivo){
            if (empty($campoArquivo)) {
                return <<<HTML
                        <img src="/imagens/pnld/p_vermelho.gif" title="Sem Termo de adesão">
HTML;
            }
            return <<<HTML
                    <img src="/imagens/pnld/p_verde.gif" title="Com Termo de adesão">
HTML;
        }

        function formataPresencaArquivoAto($campoArquivo){
            if (empty($campoArquivo)) {
                return <<<HTML
                        <img src="/imagens/pnld/p_vermelho.gif" title="Sem Ato Legal da Comissão">
HTML;
            }
            return <<<HTML
                    <img src="/imagens/pnld/p_verde.gif" title="Com Ato Legal da Comissão">
HTML;
        }
function formataPresencaArquivoMpl($campoArquivo){
            if (empty($campoArquivo)) {
                return <<<HTML
                        <img src="/imagens/pnld/p_vermelho.gif" title="Sem Minuta do PL">
HTML;
            }
            return <<<HTML
                    <img src="/imagens/pnld/p_verde.gif" title="Com Minuta do PL">
HTML;
        }
        
        $cols = "est.estdescricao, spc.spcdsc, spm.spmdsc, pce.pcepcr, pce.pceadesao, pce.pceato, pce.pcempl ";
        $where = " where 1=1 ";
        if($_POST['estuf']) { $where .= " and pce.estuf  = '".$_POST['estuf']."'";  }
        if($_POST['esdid']) { $where .= " and doc.esdid  = ".$_POST['esdid'];  }
        if($_POST['esdid2']) { $where .= " and doc2.esdid  = ".$_POST['esdid2'];  }
        
        $where .= ('t' == $_REQUEST['pcepcr'])?' AND pce.pcepcr IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcepcr'])?' AND pce.pcepcr IS NULL':'';

        $where .= ('t' == $_REQUEST['pceadesao'])?' AND pce.pceadesao IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pceadesao'])?' AND pce.pceadesao IS NULL':'';

        $where .= ('t' == $_REQUEST['pceato'])?' AND pce.pceato IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pceato'])?' AND pce.pceato IS NULL':'';

        $where .= ('t' == $_REQUEST['pcempl'])?' AND pce.pcempl IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcempl'])?' AND pce.pcempl IS NULL':'';

        $sql = <<<DML
            select
                pce.pceid,
                {$cols}
            from sase.planocarreiraprofessorestado pce
            inner join territorios.estado est on pce.estuf = est.estuf
            left join seguranca.usuario usu on usu.usucpf = pce.usucpf
            left join workflow.documento doc on pce.docid = doc.docid
            left join workflow.documento doc2 on pce.docid2 = doc2.docid
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
            {$where}
            order by est.estdescricao
DML;
        $list = new Simec_Listagem();
        $list->setQuery($sql)
            ->setCabecalho(array('Estado', 'Situação Plano de Carreira', 'Situação Adequação', 'PCR', 'Termo adesão','Ato Legal da Comissão', 'Minuta PL'))
            ->addAcao('edit', array(
                'func' => 'editarPlanodeCarreira',
                'extra-params' => array(
                    'pceid'
                )
            ))
            ->addCallbackDeCampo('pcepcr', 'formataPresencaArquivoPcr')
            ->addCallbackDeCampo('pceadesao', 'formataPresencaArquivoAdesao')
            ->addCallbackDeCampo('pceato', 'formataPresencaArquivoAto')
            ->addCallbackDeCampo('pcempl', 'formataPresencaArquivoMpl')
            //->setCampos(array('estdescricao', 'mundescricao', 'usunome'))
            ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }


    public function getLegenda(Array $estuf, $tpdid){
        //legenda por estado sase_mapa_planodecarreira.php
        global $db;
        if(count($estuf) != 0) {
            $html = <<<HTML
                <div id="legendaMapaContainer">
                <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
HTML;
        } else {
            $html = <<<HTML
                <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
                    <div id="tituloLegenda">
                        <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                        <div style="float:right;"></div>
                        <div style="clear:both;height:1px;">&nbsp;</div>
                    </div>
                    <ul>
HTML;

        }
        $where = $estuf != '' && count($estuf) > 0 ? " where pce.estuf in ('".implode("','", $estuf)."') " : "";
        $sql = "";

        switch ($tpdid){
            case '238':
                $sql = <<<DML
                    with temp_doc as (select
                                pce.pceid,
                                case
                                    when pce.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessorestado pce
                            left join workflow.documento doc on pce.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            {$where}
                            order by pceid)
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
                                pce.pceid,
                                case
                                    when pce.docid2 is null then (select spmid from sase.sitplanomunicipio where spmid = 1)
                                    else spm.spmid
                                end as spmid
                            from sase.planocarreiraprofessorestado pce
                            left join workflow.documento doc on pce.docid2 = doc.docid
                            left join sase.sitplanomunicipio spm on spm.esdid = doc.esdid
                            {$where}
                            order by pceid)
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
                                WHEN pcepcr IS NOT NULL THEN '#006600'
                                WHEN pcepcr IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pcepcr IS NOT NULL THEN 'Com PCR'
                                WHEN pcepcr IS NULL THEN 'Sem PCR'
                            END as spcdsc
                        FROM sase.planocarreiraprofessorestado pce
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
                                WHEN pceadesao IS NOT NULL THEN '#006600'
                                WHEN pceadesao IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pceadesao IS NOT NULL THEN 'Com Termo de Adesão'
                                WHEN pceadesao IS NULL THEN 'Sem Termo de Adesão'
                            END as spcdsc
                        FROM sase.planocarreiraprofessorestado pce
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
                                WHEN pceato IS NOT NULL THEN '#006600'
                                WHEN pceato IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pceato IS NOT NULL THEN 'Com Ato Legal da Comissão'
                                WHEN pceato IS NULL THEN 'Sem Ato Legal da Comissão'
                            END as spcdsc
                        FROM sase.planocarreiraprofessorestado pce
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
        $cols = "est.estdescricao, spc.spcdsc, spm.spmdsc, pce.pcepcr, pce.pceadesao, pce.pceato, pce.pcempl ";
        $where = " where 1=1 ";
        if($_POST['estuf']) { $where .= " and pce.estuf  = '".$_POST['estuf']."'";  }
        if($_POST['esdid']) { $where .= " and doc.esdid  = ".$_POST['esdid'];  }
        if($_POST['esdid2']) { $where .= " and doc2.esdid  = ".$_POST['esdid2'];  }

        $where .= ('t' == $_REQUEST['pcepcr'])?' AND pce.pcepcr IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcepcr'])?' AND pce.pcepcr IS NULL':'';

        $where .= ('t' == $_REQUEST['pceadesao'])?' AND pce.pceadesao IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pceadesao'])?' AND pce.pceadesao IS NULL':'';

        $where .= ('t' == $_REQUEST['pceato'])?' AND pce.pceato IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pceato'])?' AND pce.pceato IS NULL':'';

        $where .= ('t' == $_REQUEST['pcepml'])?' AND pce.pcempl IS NOT NULL':'';
        $where .= ('f' == $_REQUEST['pcempl'])?' AND pce.pcempl IS NULL':'';

        $sql = "
            select
                --pce.pceid,
                {$cols}
            from sase.planocarreiraprofessorestado pce
            inner join territorios.estado est on pce.estuf = est.estuf
            left join seguranca.usuario usu on usu.usucpf = pce.usucpf
            left join workflow.documento doc on pce.docid = doc.docid
            left join workflow.documento doc2 on pce.docid2 = doc2.docid
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
            {$where}
            order by est.estdescricao";

        $cabecalho = array('Estado', 'Situação Plano de Carreira', 'Situação Adequação', 'PCR', 'Termo adesão', 'Ato Legal da Comissão', 'Minuta do PL' );
        $alinhamento = array('left','left','left', "left",'left','left','left');
        $larguras = array('5%','5%','65%', '5%', '5%','5%','5%','5%');

        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=simec_sase_plano_de_carreira_" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=simec_sase_plano_de_carreira_" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");

        $db->monta_lista($sql,$cabecalho,1000000,5,'N','','N','listaTerritorio',$larguras,$alinhamento);
    }
    /**
	 * Monta legenda para o Brasil com a situação - macrocategoria
	 *
	 * @author Sávio Resende
	 * @return html
	 * @todo otimizar atraves da query
	 */
    public function montaLegendaPlanoCarreiraEstados($tipoinfo = Mapas::TIPO_ADEQUACAO)
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
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosEstadosPlanoCarreiraPCR.php';
                        $classeDados = 'DadosEstadosPlanoCarreiraPCR';
                        break;
                    case Mapas::ADESAO:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosEstadosPlanoCarreiraAdesao.php';
                        $classeDados = 'DadosEstadosPlanoCarreiraAdesao';
                        break;
                    case Mapas::ATO:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosEstadosPlanoCarreiraAto.php';
                        $classeDados = 'DadosEstadosPlanoCarreiraAto';
                        break;
                    case Mapas::PLANO_CARREIRA:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosEstadosPlanoCarreira.php';
                        $classeDados = 'DadosEstadosPlanoCarreira';
                        break;
                    case Mapas::ADEQUACAO:
                    default:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadosEstadosPlanoCarreiraAdequacao.php';
                        $classeDados = 'DadosEstadosPlanoCarreiraAdequacao';
                }
                $legenda = (new $classeDados())->dadosDaLegenda();
                foreach ($legenda as $key => $value) {
                $total = (($value['total']!='')?$value['total']:'0');
                echo <<<HTML
                            <li>
                                <table>
                                    <tr>
                                        <td>
                                            <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$total}</b>&nbsp;&nbsp;
                                        </td>
                                        <td class='dsc'>{$value['descricao']}</td>
                                    </tr>
                                </table>
                            </li>
HTML;
                }
                ?>
            </ul>
        </div>
    <?php }
}

