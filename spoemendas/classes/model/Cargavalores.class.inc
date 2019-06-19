<?php

/**
 * Classe de mapeamento da entidade spoemendas.cargavalores.
 *
 * @version $Id: Cargavalores.class.inc 132139 2017-10-18 20:45:43Z fernandoaires $
 * @since 2016.04.14
 */

/**
 * Spoemendas_Model_Cargavalores: sem descricao
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Cargavalores($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Cargavalores($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property numeric $vlrlimiteempenho
 * @property double precision $nevalor
 * @property \Datetime(Y-m-d H:i:s) $nedata
 * @property string $necod
 * @property double precision $nlvalor
 * @property \Datetime(Y-m-d H:i:s) $nldata
 * @property string $nlcod
 * @property double precision $vlrsaldoapagar
 * @property double precision $vlrpago
 * @property double precision $vlrempenhocancelado
 * @property double precision $vlrempenho
 * @property string $emecod
 * @property string $emeano
 */
class Spoemendas_Model_Cargavalores extends Modelo {

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.cargavalores';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'vlrlimiteempenho' => null,
        'nevalor' => null,
        'nedata' => null,
        'necod' => null,
        'nlvalor' => null,
        'nldata' => null,
        'nlcod' => null,
        'vlrsaldoapagar' => null,
        'vlrpago' => null,
        'vlrempenhocancelado' => null,
        'vlrempenho' => null,
    	'datareferencia' => null,
        'emecod' => null,
        'emeano' => null,
        'ptres' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array()) {
        return array(
            'vlrlimiteempenho' => array('allowEmpty' => true),
            'nevalor' => array('allowEmpty' => true),
            'nedata' => array('allowEmpty' => true),
            'necod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 20))),
            'nlvalor' => array('allowEmpty' => true),
            'nldata' => array('allowEmpty' => true),
            'nlcod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 20))),
            'vlrsaldoapagar' => array('allowEmpty' => true),
            'vlrpago' => array('allowEmpty' => true),
            'vlrempenhocancelado' => array('allowEmpty' => true),
            'vlrempenho' => array('allowEmpty' => true),
        	'datareferencia' => array('allowEmpty' => true),
            'emecod' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'emeano' => array(new Zend_Validate_StringLength(array('max' => 4))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar() {
        $this->vlrlimiteempenho = str_replace(['.', ','], ['', '.'], $this->vlrlimiteempenho);
        $this->nlvalor = str_replace(['.', ','], ['', '.'], $this->nlvalor);
        $this->nevalor = str_replace(['.', ','], ['', '.'], $this->nevalor);
        $this->vlrsaldoapagar = str_replace(['.', ','], ['', '.'], $this->vlrsaldoapagar);
        $this->vlrpago = str_replace(['.', ','], ['', '.'], $this->vlrpago);
        $this->vlrempenhocancelado = str_replace(['.', ','], ['', '.'], $this->vlrempenhocancelado);
        $this->vlrempenho = str_replace(['.', ','], ['', '.'], $this->vlrempenho);

        return parent::antesSalvar();
    }

    public function possuiDados() {
        global $db;
        $sql = "SELECT COUNT(0) as total FROM spoemendas.cargavalores ";
        if ($db->pegaUm($sql) > 0)
            return true;
        else
            return false;
    }

	public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false)
    {
        if (!$this->antesSalvar()) {
            return false;
        }

		$troca = array("'", "\\");
		
		$aryDados = array_filter($this->arAtributos, 'strlen');

		if(is_array($aryDados)){
			foreach ($aryDados as $campo => $valor){
				$arCampos[] = $campo;
				$valor = str_replace($troca, "", $valor);
				$arValores[] = trim(pg_escape_string($valor));
			}
		}

        if (count($arValores)){
			$sql = "INSERT INTO $this->stNomeTabela (".implode(', ', $arCampos).") VALUES ('".implode("', '", $arValores)."')";
		}
		
		return $this->executar($sql);
	}
}
