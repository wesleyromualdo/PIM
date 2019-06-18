<?php

/**
 * Classe de manipulação da tabela sase.estruturacarreiraremuneracao
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class Atividcronogramaredeae extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.atividcronogramaredeae";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("acrid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acrid' => null,
        'ravid' => null,
        'acrdsc' => null,
        'acrperiodo' => null
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
        'acrid',
        'ravid',
        'acrdsc',
        'acrperiodo'
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
                    acrid,
                    ravid,
                    acrdsc,
                    acrperiodo
                From sase.atividcronogramaredeae
                where ravid = {$ravid}
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
            ->setCabecalho(array('Descrição da Atividade', 'Período'))
            ->setCampos(array('acrdsc', 'acrperiodo'))
            ->esconderColunas(array('acrid', 'ravid'))
            ->addCallbackDeCampo('mundescricao', 'retornaMunicipioComprometimentoReceitaPessoal')
            ->setLarguraColuna(array('acrdsc' => '50%', 'acrperiodo' => '50%'));

        if($campos) {
            $lista
//                ->addCallbackDeCampo('acrdsc', 'getDescricaoAtividadeCronograma')
//                ->addCallbackDeCampo('acrperiodo', 'getPeriodoAtividadeCronograma')
                ->addAcao('delete', array(
                    'func' => 'apagarAtividade',
                    'titulo' => 'Apagar atividade',
                    'extra-params' => array(
                        'acrid',
                        'ravid'
                    )
                ))
                ->addAcao('edit', array(
                    'func' =>  'editaAtividade',
                    'titulo' => 'Edita atividade',
                    'extra-params' => array(
                        'acrid'
                    )
                ));
        }

        $lista
            ->setFormOff();

        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
}