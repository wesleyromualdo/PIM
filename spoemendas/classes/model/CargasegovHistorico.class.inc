<?php
/**
 * Classe de mapeamento da entidade spoemendas.cargasegov_historico.
 *
 * @version $Id$
 * @since 2017.09.12
 */

/**
 * Spoemendas_Model_CargasegovHistorico: sem descricao
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Guilherme Félix Da Silva Maciel <guilherme.maciel@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_CargasegovHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_CargasegovHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hcaid chave primária da tabelacargasegov_historico
 * @property  $usucpf CPF do respnsável pela alteração
 * @property  $hcadatcriacao Data de criação do log
 */
class Spoemendas_Model_CargasegovHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.cargasegov_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hcaid',
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
        'hcaid' => null,
        'usucpf' => null,
        'hcadatcriacao' => null
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Spoemendas_Model_Cargasegov $model)
    {
    
        $this->arAtributos['usucpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hcadatcriacao'] = date('Y-m-d H:m:s');
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
	
	public function gerarHistorico()
	{
		$this->arAtributos['usucpf'] = $_SESSION['usucpf'];
		$this->arAtributos['hcadatcriacao'] = date('Y-m-d H:m:s');
		//grava no histórico
		$this->popularDadosObjeto($this->arAtributos);
		$this->salvar();
		
		$sql = <<<DML
		SELECT max(hcaid) FROM spoemendas.cargasegov_historico;
DML;
		$historico = $this->pegaUm($sql);
		return $historico;
	}

	public function pegaDataUltimaCarga() {
        $sql = <<<SQL
            SELECT
              TO_CHAR(hcadatcriacao, 'DD/MM/YYYY') AS hcadatcriacaof
            FROM spoemendas.cargasegov_historico
            ORDER BY hcadatcriacao
            DESC limit 1
SQL;
        return $this->pegaUm($sql);
    }
}
