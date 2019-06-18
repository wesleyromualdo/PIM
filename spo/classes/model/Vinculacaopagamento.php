<?php
/**
 * Classe de mapeamento da entidade spo.vinculacaopagamento.
 *
 * @version $Id: Vinculacaopagamento.php 130733 2017-09-19 16:42:30Z saulocorreia $
 */

/**
 * Spo_Model_Vinculacaopagamento.
 *
 * @package  Spo\Model
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Spo_Model_Vinculacaopagamento extends Modelo
{
    /**
     * @var string Nome da tabela especificada.
     */
    protected $stNomeTabela = 'spo.vinculacaopagamento';

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
        'vnpcod' => null,
        'vnpdsc' => null,
        'vnpano' => null,
    );

    public static function getQueryCombo($exercicio, $whereAdicional = '')
    {
        $sql = <<<DML
    SELECT vp.vnpcod AS codigo, vp.vnpcod || ' - ' || vp.vnpdsc AS descricao
FROM spo.vinculacaopagamento vp
    WHERE vp.vnpano = '{$exercicio}'
    {$whereAdicional}
ORDER BY vp.vnpcod
DML;
        return $sql;
    }

	public function carregarVinculacoes($exercicio)
	{
		if (!($data = $this->carregar(self::getQueryCombo($exercicio)))) {
			return [];
		}

		$vinculacoes = [];
		foreach ($data as $vinculacao) {
			$vinculacoes[$vinculacao['codigo']] = $vinculacao['descricao'];
		}
		return $vinculacoes;
	}

}
