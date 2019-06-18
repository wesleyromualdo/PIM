<?php
/**
 * Classe de mapeamento da entidade sase.territoriomunicipio.
 *
 * @version $Id$
 * @since 2017.06.08
 */

/**
 * Model_Territoriomunicipio: tabela dos municio vinculado a infirmaçoes terri
 *
 * @package Model\Territoriomunicipio
 * @uses Simec\Db\Modelo
 * @author Gabriel Cabral De Almeida <gabriel.almeida@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Territoriomunicipio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Territoriomunicipio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $termunid chave primaria de tabela  - default: nextval('sase.territoriomunicipio_termunid_seq'::regclass)
 * @property int $infterid chave estrangerira da tabela sase.informacoesterritorio
 * @property string $muncod cheve estrangeira da tabela territorio.municipio
 * @property string $usucpf cpf do usuario quee cadastrou
 * @property string $terstatus  - default: 'A'::bpchar
 */
class Territoriomunicipio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.territoriomunicipio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'termunid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'infterid' => array('tabela' => 'sase.informacoesterritorio', 'pk' => 'infterid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'termunid' => null,
        'infterid' => null,
        'muncod' => null,
        'usucpf' => null,
        'terstatus' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'termunid' => array('Digits'),
            'infterid' => array('allowEmpty' => true,'Digits'),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'terstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    function salvarTerritorioMunicipio($arrDados){



        try{
            $sql ="INSERT INTO sase.territoriomunicipio ( infterid, muncod, usucpf, terstatus) VALUES ({$arrDados[infterid]}, '{$arrDados[muncod]}', '{$arrDados[usucpf]}', 'A'); ";
            $this->executar($sql);
            $this->commit();
        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }
        return true;
    }

    function deleteTerritorioMunicipio(){

        try{
            $sql ="DELETE FROM sase.territoriomunicipio WHERE termunid = {$_POST['termunid']} ";
            $this->executar($sql);
            $this->commit();
        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }
        return true;
    }
}
