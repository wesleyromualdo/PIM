<?php
/**
 * Sase_Model_Pneprev
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
require_once APPRAIZ . "sase/classes/Planocarreiraprofessor.class.inc";

class Sase_Model_Municipiosassistidosredeae extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.municipiosassistidosredeae';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'marid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'subid' => array('tabela' => 'sase.pnedial', 'pk' => 'pneiddial'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'marid' => null,
        'ravid' => null,
        'assid' => null,
        'pcpid' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'marid' => array(  'Digits'  ),
            'ravid' => array(  'Digits'  ),
            'assid' => array(  'Digits'  ),
            'pcpid' => array(  'Digits'  )
        );
    }//end getCamposValidacao($dados)

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        $this->popularDadosObjeto();

        try{
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            return false;
        }

        if($sucesso){
            return true;
        }
        return false;
    }//end gravar()

    public function carrega($marid){
        return new Sase_Model_Municipiosassistidosredeae($marid);
    }

    public function montaSqlRelatorio($ravid){
        if( !empty($ravid) || is_numeric($ravid) ){
            $sql = <<<DML
                select
                    mar.marid,
                    pcp.pcpid,
                    pcp.docid,
                    pcp.docid2,
                    mun.mundescricao
                from sase.municipiosassistidosredeae mar
                inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                where ravid = {$ravid}
                order by mun.mundescricao
DML;
            return $sql;
       
        }else{
            return false;
        }
        
    }

    public function getTabelaRelatorioTecnico($ravid, $usucpf){

        function enumeraLinha($valor, $linha)
        {
            if(!empty($linha['pcpid'])){
                if(empty($linha['docid2'])) {
                    $docid = wf_cadastrarDocumento(TPDID_SASE_SITUACAO_PLANOCARREIRA, 'Documento Avaliador Educacional - DIVAPE');
                    $pcp = new Sase_Model_Planocarreiraprofessor();
                    $pcp->carregarPorId($linha['pcpid']);
                    $pcp->docid2 = $docid;
                    $pcp->salvar();
                    $pcp->commit();
                }
                if(empty($linha['docid'])){
                    $docid = wf_cadastrarDocumento(TPDID_SASE_PLANODECARREIRA, 'Documento Avaliador Educacional - DIVAPE');
                    $pcp = new Sase_Model_Planocarreiraprofessor();
                    $pcp->carregarPorId($linha['pcpid']);
                    $pcp->docid = $docid;
                    $pcp->salvar();
                    $pcp->commit();
                }
            }
            static $i = 1;
            return str_pad($i++, 2, '0', STR_PAD_LEFT);
        }

        $sql = $this->montaSqlRelatorio($ravid);
        if(!$sql){return false;}

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Municípios assistidos em ordem alfabética' => array('','')))
            ->setCampos(array('mundescricao'))
            ->setId('marid')
            ->esconderColunas(array('pcpid', 'docid2', 'docid'))
            ->addCallbackDeCampo('marid', 'enumeraLinha')
            ->setFormOff();
        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function montaSqlResultadosConsolidadosPCP($ravid){
        global $db;
        $tpdid = TPDID_SASE_SITUACAO_PLANOCARREIRA;
        $sql = <<<DML
            select
                esdid
            from workflow.estadodocumento
            where tpdid = {$tpdid}
            order by esdordem
DML;
        $esdDocs = $db->carregar($sql);
        
            $sql = <<<DML
                select
                    mar.marid,
                    mun.mundescricao,
                    doc.esdid,
                    (
                        select esdid from sase.sitatualmunicipiotecredeae stm where stm.marid = mar.marid and stm.esdid in (select esdid from workflow.estadodocumento where tpdid = {$tpdid}) order by marid desc limit 1
                    ) stmesdid
                from sase.municipiosassistidosredeae mar
                left join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                left join workflow.documento doc on pcp.docid2 = doc.docid
                where ravid = {$ravid}
DML;
            $marIds = $db->carregar($sql);

            if (!$marIds) {
                return array();
            }

            $dados = array();
            $campo = 1;
            $chave = 0;
            foreach ($marIds as $marid) {
                $dados[$chave] = array();
                $dados[$chave]['marid'] = $marid['marid'];
                $dados[$chave]['mundescricao'] = $marid['mundescricao'];
                foreach ($esdDocs as $esdid) {
                    $esdidMar = empty($marid['stmesdid']) ? $marid['esdid'] : $marid['stmesdid'];
                    $dados[$chave]['campo'.$campo] = $esdid['esdid'] === $esdidMar ? 'X' : '';
                    $dados[$chave]['esdid'.$campo] = $esdid['esdid'];
                    $campo++;
                }
                $campo = 1;
                $chave++;
            }

            return $dados;

    }

    public function montaCabecalhoResultadosConsolidadosPCP(){
        global $db;
        $tpdid = TPDID_SASE_SITUACAO_PLANOCARREIRA;
        $cabecalho = array('Município');

        $sql = <<<DML
            select
                esddsc
            from workflow.estadodocumento
            where tpdid = {$tpdid}
            order by esdordem
DML;
        $res = $db->carregar($sql);

        foreach ($res as $r) {
            $cabecalho[] = $r['esddsc'];
        }

        return $cabecalho;
    }

    public function getResultadosConsolidadosPCP($ravid){
        if( !empty($ravid) || is_numeric($ravid) ){
            $dados = $this->montaSqlResultadosConsolidadosPCP($ravid);
        }
        
        $cabecalho = $this->montaCabecalhoResultadosConsolidadosPCP();

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setDados($dados)
            ->setId('tb_resultadoconsolidado')
            ->setCabecalho($cabecalho)
            ->esconderColunas(array('marid', 'esdid1', 'esdid2', 'esdid3', 'esdid4', 'esdid5', 'esdid6', 'esdid7', 'esdid8', 'esdid9'))
            ->setCampos(array('mundescricao', 'campo1', 'campo2', 'campo3', 'campo4', 'campo5', 'campo6', 'campo7', 'campo8', 'campo9'))
            ->setFormOff();
        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function montaSqlMunAlteracao($ravid, $usucpf, $dtini, $dtfim, $tipo = 'c'){
        $usucpf = empty($usucpf) ? $_SESSION['usucpf'] : $usucpf;
        $n = $tipo != 'c' ? 'not' : '';
        $sql = <<<DML
            select distinct
                mun.mundescricao,
                case
                    when esdo.esddsc is null then 'Sem histórico'
                    when esdo.esddsc is not null then esdo.esddsc
                end as esddsco,
                case
                    when esd.esddsc is null then 'Sem histórico'
                    when esd.esddsc is not null then esd.esddsc
                end as esddsc,
                esd.esdordem,
                esdo.esdid,
                esd.esdid as esdido,
                mar.marid
            from sase.planocarreiraprofessor pcp
            inner join sase.municipiosassistidosredeae mar on pcp.pcpid = mar.pcpid and mar.ravid = {$ravid}
            left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid and mst.mstid = (select max(mstid) from sase.municipiossemalteracaoperiodotecredeae where marid = mar.marid)
            left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
            inner join workflow.historicodocumento hst on hst.docid = pcp.docid2
                and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = pcp.docid2 and date(htddata) <= date('{$dtfim}'))
            inner join workflow.acaoestadodoc aed on aed.aedid = hst.aedid
            inner join workflow.estadodocumento esdo on aed.esdidorigem = esdo.esdid
            inner join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino
            inner join sase.usuarioresponsabilidade ur on pcp.muncod = ur.muncod
            inner join territorios.municipio mun on mun.muncod = pcp.muncod
            where ur.usucpf = '{$usucpf}'
            and ur.rpustatus = 'A'
            and {$n} exists (
                select
                    1
                from workflow.historicodocumento doc
                where doc.docid = pcp.docid2
                and date(doc.htddata) between date('{$dtini}') and date('{$dtfim}')
            )
            order by esd.esdordem
DML;
        return $sql;
    }

    public function getMunAlteracao($ravid, $usucpf, $dtini, $dtfim, $tipo){
        if (!empty($ravid) || is_numeric($ravid)){
            $sql = $this->montaSqlMunAlteracao($ravid, $usucpf, $dtini, $dtfim, $tipo);
            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setQuery($sql)
                ->setCabecalho(array('Município', 'Situação Anterior', 'Situação Atual'))
                ->setCampos(array('mundescricao', 'esddsco', 'esddsc',))
                ->esconderColunas(array('esdordem', 'esdid', 'esdido', 'marid'))
                ->setFormOff();
            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }else{
            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setDados($dados)
                ->setCabecalho(array('Município', 'Situação Anterior', 'Situação Atual'))
                ->setCampos(array('mundescricao', 'esddsco', 'esddsc',))
                ->setFormOff();
            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }

    public function montaSqlMunSemAlteracao($ravid, $usucpf, $dtini, $dtfim){
        $pfls = arrayPerfil($usucpf); //$_SESSION['perfil'][$_SESSION['usucpf']][$_SESSION['sisid']];
        $diretoria = empty($_GET['diretoria']) ? 1 : $_GET['diretoria'];
        $pflcod = "";
        if($diretoria == '2') {
            if (in_array(PFLCOD_SASE_TECNICO_DIVAPE, $pfls)) {
                $pflcod = PFLCOD_SASE_TECNICO_DIVAPE;
            }
            if (in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $pfls)) {
                $pflcod = PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE;
            }
        } else {
            if (in_array(PFLCOD_SASE_TECNICO, $pfls)) {
                $pflcod = PFLCOD_SASE_TECNICO;
            }
            if (in_array(PFLCOD_SASE_EXECUTIVO, $pfls)) {
                $pflcod = PFLCOD_SASE_EXECUTIVO;
            }
        }

        if (!$pflcod) {
            $pflcod = implode(',', $pfls);
        }

        $sql = <<<DML
            select
                mar.marid,
                mst.mstid,
                mun.mundescricao,
                case
                    when esd.esddsc is null then 'Sem Informações*'
                    else esd.esddsc
                end as esddsc,
                esd.esdid,
                mst.msaobservacoes
            from sase.planocarreiraprofessor pcp
            inner join territorios.municipio mun on pcp.muncod = mun.muncod
            inner join sase.municipiosassistidosredeae mar on pcp.pcpid = mar.pcpid and mar.ravid = {$ravid}
            inner join sase.usuarioresponsabilidade ur on pcp.muncod = ur.muncod
            left join workflow.documento doc on pcp.docid2 = doc.docid
            left join workflow.estadodocumento esd on doc.esdid = esd.esdid
            left join sase.municipiossemalteracaoperiodotecredeae mst on mar.marid = mst.marid and doc.esdid = mst.esdid
            where ur.usucpf = '{$usucpf}'
            and ur.rpustatus = 'A'
            and ur.pflcod in ({$pflcod})
            and not exists (
                select
                    1
                from workflow.historicodocumento doc
                where doc.docid = pcp.docid2
                and date(doc.htddata) between date('{$dtini}') and date('{$dtfim}')
            )
            order by mundescricao
DML;
        return $sql;
    }

    public function getMunSemAlteracao($ravid, $usucpf, $dtini, $dtfim, $perCampos){
        if ( !empty($ravid) || is_numeric($ravid)) {
            $sql = $this->montaSqlMunSemAlteracao($ravid, $usucpf, $dtini, $dtfim);

            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setQuery($sql)
                ->setCabecalho(array('Município', 'Etapa Atual', 'Motivo'))
                ->setCampos(array('mundescricao', 'esddsc', 'msaobservacoes',));

            if($perCampos) {
                $lista->addCallbackDeCampo('msaobservacoes', 'getObsMunicipiosSemAlteracao');
            }

            $lista->esconderColunas(array('esdid', 'marid', 'mstid'))
                ->setFormOff();
            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }else{
            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setDados($dados)
                ->setCabecalho(array('Município', 'Etapa Atual', 'Motivo'))
                ->setCampos(array('mundescricao', 'esddsc', 'msaobservacoes',))
                ->setFormOff();
            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }

    protected function gerarTabela($dados, $perCampos = true)
    {
        if(!empty($dados)){
            global $db;

            $txtArea = <<<HTML
                <textarea name="pamdsc%s" id="pamdsc%s" class="form-control" cols="50" rows="2" maxlength="200">%s</textarea>
HTML;

            //$tabelaHtml = new HTML_Table('table_prazo_pne', 'table table-condensed table-bordered  table-responsive');
            $tabelaHtml = new HTML_Table('table_prazo_pne', 'table table-striped table-bordered table-hover table-condensed tabela-listagem');
            $camposDaTabela = array('mundescricao' => 'Município');
    //        $tabelaHtml->addTSection('thead');
    //        $tabelaHtml->addRow();
    //
    //        //header
    //        foreach ($camposDaTabela as $atributo => $titulo) {
    //            $tabelaHtml->addCell($titulo, '', 'header', array());
    //        }

            //body

            $esdid = '';
    //        ver($dados, d);
            $t = 0;
            $tg = 0;
            foreach ($dados as $valor) {

                if($valor['esdid'] != $esdid){
                    if($esdid != ''){
                        $tabelaHtml->addRow();
                        $tabelaHtml->addCell('Total: '.$t, '', 'header', array("style"=>"text-align: left;", "colspan"=>"2"));
                    }
                    $tabelaHtml->addRow();
                    $tabelaHtml->addCell($valor['spmdsc'], '', 'header', array("style"=>"text-align: center;", "colspan"=>"2"));
                    $tabelaHtml->addRow();
                    $tabelaHtml->addCell('Município', '', 'header', array("style"=>"text-align: center;", "width"=>"50%"));
                    $tabelaHtml->addCell('Ações Propostas', '', 'header', array("style"=>"text-align: center;"));
                    $esdid = $valor['esdid'];
                    $t = 0;
                }

                $tabelaHtml->addTSection('tbody');
                $tabelaHtml->addRow();
                foreach ($camposDaTabela as $atributo => $titulo) {
                    $value = $valor[$atributo];
                    $attr = array('class' => 'text-center');
                    if($value != '') {
                        $tabelaHtml->addCell($value, '', 'data', $attr);
                        if($perCampos) {
                            $tabelaHtml->addCell(sprintf($txtArea, $valor['marid'], $valor['marid'], $valor['pamdsc']), '', 'data', $attr);
                        } else {
                            $tabelaHtml->addCell($valor['pamdsc'], '', 'data', $attr);
                        }
                    } else {
                        $attr['colspan'] = '2';
                        $tabelaHtml->addCell('Sem municípios nessa situação', '', 'data', $attr);
                    }
                }
                if(!empty($valor['mundescricao'])) { $t++; $tg++; }
            }
            $tabelaHtml->addRow();
            $tabelaHtml->addCell('Total: '.$t, '', 'header', array("style"=>"text-align: left;", "colspan"=>"2"));
            $tabelaHtml->addRow();
            $tabelaHtml->addCell('Total Geral: '.$tg, '', 'header', array("style"=>"text-align: left;", "colspan"=>"2"));
        }else{
            $tabelaHtml = new HTML_Table('tb_render', 'table table-bordered table-hover table-condensed  tabela-listagem table-responsive');
            $tabelaHtml->addTSection('tbody');
            $tabelaHtml->addTSection('thead');
            $tabelaHtml->addRow('',array('class'=>'lista-vazia'));
            $tabelaHtml->addCell('', '', '', array("colspan"=>"3"));
            echo '<div class="alert alert-info col-lg-8 col-lg-offset-2" role="alert" style="font-size:16px">';
            echo '<center>Nenhum registro encontrado</center>';
            echo '</div>';
        }
        
        return $tabelaHtml->display();
    }

    public function montaSqlAcoesDesenvMun($ravid){
        $sql = <<<DML
            with temp_mun as (select
                        mun.mundescricao,
                        pcp.muncod,
                        esd.esdid,
                        mar.marid,
                        pam.pamid,
                        pam.pamdsc
                    from sase.municipiosassistidosredeae mar
                    inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                    inner join territorios.municipio mun on pcp.muncod = mun.muncod
                    left join workflow.documento doc on pcp.docid2 = doc.docid
                    left join workflow.estadodocumento esd on doc.esdid = esd.esdid
                    left join sase.pcpacoesmunicipio pam on pam.marid = mar.marid and pam.esdid = esd.esdid
                    where mar.ravid = {$ravid})

            select
                marid,
                spm.esdid,
                spmdsc,
                mundescricao,
                pamdsc,
                pamid
            from sase.sitplanomunicipio spm
            left join temp_mun mun on spm.esdid = mun.esdid
            order by mundescricao
DML;
        return $sql;
    }

    public function getAcoesDesenvMun($ravid, $perCampos = true){
        global $db;
        if(!empty($ravid) || is_numeric($ravid)){
            $sql = $this->montaSqlAcoesDesenvMun($ravid);
            $dados = $db->carregar($sql);

        }
        return $this->gerarTabela($dados, $perCampos);
    }

}