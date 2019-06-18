<?php
/**
 * Classe de mapeamento da entidade seguranca.perfilvinculado.
 *
 * @version $Id$
 * @since 2017.02.16
 */

/**
 * Seguranca_Model_Perfilvinculado: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Perfilusuario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Perfilusuario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpf
 * @property int $pflcod
 * @property int $pflvinculado
 */
class Seguranca_Model_Seguranca_PerfilVinculado extends Modelo
{
    protected $tabelaAssociativa = true;

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.perfilvinculado';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'pflcod',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'pflcod' => null,
        'pflvinculado' => null,
    );

    public function salvarPerfilVinculado($pflcod, $pflvinculados)
    {
        if(!$pflvinculados) {
            $query = "DELETE FROM seguranca.perfilvinculado WHERE pflcod = {$pflcod}";
            return $this->executar($query, false);
        }

        $pflvinculados = '{' . implode(",", $pflvinculados) . '}';
        $query = "UPDATE seguranca.perfilvinculado SET pflvinculado = '{$pflvinculados}' WHERE pflcod = {$pflcod} returning pflcod;";

        $pflVinculado = $this->recuperar("SELECT pflcod FROM seguranca.perfilvinculado WHERE pflcod = {$pflcod}");
        if (!$pflVinculado) {
            $query = "INSERT INTO seguranca.perfilvinculado( pflcod, pflvinculado ) VALUES ('{$pflcod}', '{$pflvinculados}') returning pflcod;";
        }

        return $this->executar($query, false);
    }
}
