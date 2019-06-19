<?php

/**
 * Classe de manipulação da tabela sase.estruturacarreiraremuneracao
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class SitatualMunicipioTecredeae extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.sitatualmunicipiotecredeae";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("stmid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'stmid' => null,
        'marid' => null,
        'esdid' => null,
        'stmobservacoes' => null
    );

    /**
     * Atributos String da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosStr = array(
    );

    /**
     * @name $arCampos
     * @var array
     * @access protected
     * Campos da Tabela
     */
    protected $arCampos = array(
        'stmid',
        'marid',
        'esdid',
        'stmobservacoes'
    );

    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
    );

    /**
     * Menságens gerais
     * @name $msg
     * @var string
     * @access public
     */
    public $msg;
    protected $ravid;
    protected $arrEstadosDoc;

    /**
     * Popula Objeto com Array
     *
     * @param array $arDados
     * @return $this
     * @author Sávio Resende
     */
    public function popula( Array $arDados ){
        $this->arAtributos[$this->arChavePrimaria[0]] = $arDados[$this->arChavePrimaria[0]];
        $this->popularObjeto( $this->arCampos, $arDados );
        return $this;
    }

    public function __construct($id = null, $ravid = null){
        parent::__construct($id);
        $this->ravid = $ravid;
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
}