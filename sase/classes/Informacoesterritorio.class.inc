<?php
/**
 * Classe de mapeamento da entidade sase.informacoesterritorio.
 *
 * @version $Id$
 * @since 2017.06.02
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Sase_Model_Informacoesterritorio: tabela guarada as informações do Territorrio
 *
 * @package Sase\Model
 * @uses Simec\Db\Modelo
 * @author Gabriel Cabral De Almeida <gabriel.almeida@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_Model_Informacoesterritorio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_Model_Informacoesterritorio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $infterid id da tabela  - default: nextval('sase.informacoesterritorio_infterid_seq'::regclass)
 * @property string $infternomeorganizacao camçpo Nome da Organização
 * @property int $orgid chave estrangera para a organização (Tipo do Organização)
 * @property string $infterstatus campo guarda do status (A - Ativo , I - Inativo)
 * @property string $inftercor campo guarda a cor em hexadecimal
 */
class Sase_Model_Informacoesterritorio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.informacoesterritorio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'infterid',
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
        'infterid' => null,
        'infternomeorganizacao' => null,
        'orgid' => null,
        'infterstatus' => null,
        'inftercor' => null,
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
            'infterid' => array('Digits'),
            'infternomeorganizacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 200))),
            'orgid' => array('Digits'),
            'infterstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'inftercor' => array(new Zend_Validate_StringLength(array('max' => 7))),
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


    public function SalvarInformacoesterritorio(){
        try{
            $this->popularDadosObjeto();
            $this->salvar();
            $this->commit();
        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }
        return $this->infterid;
    }

    public function editarInformacoesterritorio(){
        try{
            $this->popularDadosObjeto();
            $this->alterar();
            $this->commit();
        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }
        return $this->infterid;
    }



}
