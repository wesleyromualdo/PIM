<?php
/**
 * Classe de mapeamento da entidade par3.contratos.
 *
 * @version $Id$
 * @since 2017.03.02
 */

/**
 * Par3_Model_Contratos: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Contratos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Contratos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $conid chave primária do contrato - default: nextval('par3.contratos_conid_seq'::regclass)
 * @property int $inuid fk da unidade
 * @property int $prgid fk do programa
 * @property numeric $connumero número do contrato
 * @property string $constatus
 * @property \Datetime(Y-m-d H:i:s) $condtcriacao  - default: now()
 * @property string $usucpf
 */
class Par3_Model_Contratos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.contratos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'conid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'conid' => null,
        'inuid' => null,
        'prgid' => null,
        'conano' => null,
        'connumero' => null,
        'constatus' => null,
        'condtcriacao' => null,
        'usucpf' => null,
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
            'conid' => array('Digits'),
            'inuid' => array('allowEmpty' => true, 'Digits'),
            'prgid' => array('allowEmpty' => true, 'Digits'),
            'connumero' => array('allowEmpty' => true),
            'constatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'condtcriacao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
        );
    }

    public function pegarSQLSelectCombo($arrPost = array())
    {
        $where = array("constatus = 'A'");

        if($arrPost['inuid']){
            $where[] = "inuid = {$arrPost['inuid']}";
        }

        $sql = "SELECT conid as codigo, connumero as descricao
                FROM ".$this->stNomeTabela."
                WHERE ".implode(' AND ', $where)."
                ORDER BY 2";

        return $sql;
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
        public
        function antesSalvar()
        {
            // -- Implemente suas transformações de dados aqui

            // -- Por padrão, o método sempre retorna true
            return parent::antesSalvar();
        }

    }
