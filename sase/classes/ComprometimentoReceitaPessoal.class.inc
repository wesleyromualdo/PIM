<?php

/**
 * Classe de manipulação da tabela sase.estruturacarreiraremuneracao
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class ComprometimentoReceitaPessoal extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.comprometimentoreceitapessoal";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("cprid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cprid' => null,
        'marid' => null,
        'esdid' => null,
        'crpcompate60' => null,
        'crpcompate6180' => null,
        'crpcompate8190' => null,
        'crpcompate91' => null
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
        'cprid',
        'marid',
        'esdid',
        'crpcompate60',
        'crpcompate6180',
        'crpcompate8190',
        'crpcompate91'
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

    public function montaSqlRelatorio(){
        $ravid = $this->ravid;

        if(isset($this->ravid)) {
            $sql = <<<DML
                select
                    mun.mundescricao,
                    mar.marid,
                    doc.esdid,
                    crp.crpid,
                    crp.crpcompate60,
                    crp.crpcompate6180,
                    crp.crpcompate8190,
                    crp.crpcompate91
                from sase.municipiosassistidosredeae mar
                inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                left join workflow.documento doc on doc.docid = pcp.docid
                left join sase.comprometimentoreceitapessoal crp on crp.marid = mar.marid
                where mar.ravid = {$ravid}
                order by mun.mundescricao
DML;
        } else {
            $this->msg = "Informe o código do relatório, campo 'ravid'";
            return false;
        }
        return $sql;

    }

    public function getTabelaRelatorio($campos = false, $layout = false){
        $sql = $this->montaSqlRelatorio();
        if(!$sql){
            return false;
        }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Municípios', 'Compromete até 60%', 'Compromete de 61% à 80%', 'Compromete de 81% à 90%', 'Compromete a partir de 91%'))
            ->setCampos(array('mundescricao', 'crpcompate60', 'crpcompate6180', 'crpcompate8190', 'crpcompate91'))
            ->esconderColunas(array('marid', 'esdid', 'crpid'))
            ->addCallbackDeCampo('mundescricao', 'retornaMunicipioComprometimentoReceitaPessoal')
            ->setLarguraColuna(array('mundescricao' => '20%', 'crpcompate60' => '20%', 'crpcompate6180' => '20%', 'crpcompate8190' => '20%', 'crpcompate91' => '20%'));

        if($campos) {
            $lista
                ->addCallbackDeCampo('crpcompate60', 'retornaCampoCompate60')
                ->addCallbackDeCampo('crpcompate6180', 'retornaCampoCompate6180')
                ->addCallbackDeCampo('crpcompate8190', 'retornaCampoCompate8190')
                ->addCallbackDeCampo('crpcompate91', 'retornaCampoCompate91');
        }

        $lista
            ->setFormOff();

        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
}