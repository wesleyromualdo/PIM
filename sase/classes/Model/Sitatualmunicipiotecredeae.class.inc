<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 04/02/2016
 * Time: 10:50
 */


include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Sitatualmunicipiotecredeae extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.sitatualmunicipiotecredeae';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'stmid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'marid' => array('tabela' => 'sase.municipiosassistidosredeae', 'pk' => 'marid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'stmid' => null,
        'marid' => null,
        'esdid' => null,
        'stmobservacoes' => null,
        'stmpspnvinculado' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'stmid' => array('allowEmpty' => false, 'Digits'),
            'marid' => array('allowEmpty' => false, 'Digits'),
            'esdid' => array('allowEmpty' => false, 'Digits'),
            'stmobservacoes' => array('allowEmpty' => true),
            'stmpspnvinculado' => array('allowEmpty' => true)
        );
    }

    public function montaEstadosDocumentos($tpdid = null, $arrEstadosFora = array()){
        global $db;
        if(!is_array($this->arrEstadosDoc)) {
            $notIn = implode(",", $arrEstadosFora);
            $sql = <<<DML
                select
                    esddsc,
                    esdid
                from workflow.estadodocumento
                where tpdid = {$tpdid}
                and esdid not in ({$notIn})
                order by esdordem
DML;
            $this->arrEstadosDoc = $db->carregar($sql);
            return $this->arrEstadosDoc;
        } else {
            return $this->arrEstadosDoc;
        }
    }

    public function montaCabecalhoTabela($tpdid = null){
        $cabecalho = array();
        $res = $this->montaEstadosDocumentos($tpdid);
        $cabecalho[] = "Município";
        foreach ($res as $r) {
            $cabecalho[] = $r['esddsc'];
        }
        $cabecalho[] = "Observações";
        return $cabecalho;
    }

    public function montaCamposTabela($tpdid = null){
        $campos = array();
        $res = $this->montaEstadosDocumentos($tpdid);
        $i = 1;
        $campos[] = 'mundescricao';
        foreach ($res as $r) {
            $campos[] = 'coluna'.$i;
            $i++;
        }
        $campos[] = 'stmobservacoes';
        return $campos;
    }

    public function montaSqlRelatorio($tpdid = null, $ratdata2periodo = null, $arrEstadosFora = array()){
        $sql = "";
        if(isset($this->ravid) and $tpdid != null and $ratdata2periodo != null){
            $ravid = $this->ravid;
            $res = $this->montaEstadosDocumentos($tpdid, $arrEstadosFora);
            $colunas = "";
            $i = 1;
            foreach ($res as $r) {
                $colunas .= <<<DML
                case
                    when aed.esdiddestino = {$r['esdid']} then 'X'
                end as coluna{$i},
DML;
                $i++;
            }

            $sql = <<<DML
                select distinct
                    mun.mundescricao,
                    pcp.docid2 as docid,
                    mar.marid,
                    stm.stmid,
                    aed.esdiddestino as esdid,
                    {$colunas}
                    stm.stmobservacoes
                from sase.planocarreiraprofessor pcp
                inner join sase.usuarioresponsabilidade rpu on rpu.estuf = pcp.estuf
                inner join sase.municipiosassistidosredeae mar on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on mun.muncod = pcp.muncod
                left join workflow.historicodocumento hst on hst.docid = pcp.docid2 and date(htddata) <= date('{$ratdata2periodo}')
                left join workflow.acaoestadodoc aed on aed.aedid = hst.aedid
                left join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid and stm.esdid = aed.esdiddestino
                where mar.ravid = {$ravid}
                order by mun.mundescricao
DML;


        }
        return $sql;
    }

    public function montaLarguraColunas($tpdid = null){
        $res = $this->montaCamposTabela($tpdid);
        $cont = count($res);
        $largura = 100 / $cont;
        $arrLargura = array();
        foreach ($res as $r) {
            $arrLargura[$r] = $largura.'%';
        }
        return $arrLargura;
    }

    public function getTabelaRelatorio($campos = false, $tpdid = null, $ratdata2periodo = null, $arrEstadosFora = array()){
        $sql = $this->montaSqlRelatorio($tpdid, $ratdata2periodo, $arrEstadosFora);
        if(!$sql){
            return false;
        }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho($this->montaCabecalhoTabela($tpdid))
            ->setCampos($this->montaCamposTabela($tpdid))
            ->setId('tbSituaAtualMun')
            ->setLarguraColuna($this->montaLarguraColunas($tpdid))
            ->esconderColunas(array('docid', 'esdid', 'marid', 'stmid'));

        if($campos) {
            $lista
                ->addCallbackDeCampo('stmobservacoes', 'getObservacoesSituaAtualMun');
        }

        $lista
            ->setFormOff();

        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    /**
     * Retorna o ultimo registro na tabela sase.sitatualmunicipiotecredeae, referente ao município assistido passado no parâmetro
     *
     * @param $marid ID do municipio assistido
     */
    public function carregaPorMarid($marid){
        $id = trim( str_replace("'", "",(string)$marid) );
        $sql = "select * from {$this->stNomeTabela} where marid = {$marid} order by stmid desc limit 1";

        $arResultado = $this->pegaLinha( $sql );
        $this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
    }

    public function carregaPorMaridEsdid($marid, $esdid){
        $id = trim( str_replace("'", "",(string)$marid) );
        $sql = "select * from {$this->stNomeTabela} where marid = {$marid} and esdid = {$esdid} order by stmid desc limit 1";

        $arResultado = $this->pegaLinha( $sql );
        if(is_array($arResultado)) {
            $this->popularObjeto(array_keys($this->arAtributos), $arResultado);
        }
    }

}