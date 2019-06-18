<?php
/**
 * Gerador de models.
 *
 * @version $Id: ModelGenerator.php 115268 2016-11-14 15:37:08Z saulocorreia $
 */

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class ModelGenerator
{
    public $schema;
    public $prefixoClasse;
    public $extensao;
    public $tabela;
    public $atributos;
    public $nomeClasse;

    public $column_name;
    public $is_nullable;
    public $data_type;
    public $character_maximum_length;
    public $constraint_name;

    private $_data;

    public function __construct(Array $properties = array())
    {
        if (!empty($properties)) {
            foreach ($properties as $key => $value) {
                $this->{$key} = $value;
            }
        }
        $this->_data = $properties;
    }

    public function setAtributos(Array $atributos = array())
    {
        if (!empty($atributos)) {
            foreach ($atributos as $key => $value) {
                $this->{$key} = $value;
            }
        }
    }

    public function __set($property, $value)
    {
        return $this->_data[$property] = $value;
    }

    public function __get($property)
    {
        return array_key_exists($property, $this->_data) ? $this->_data[$property] : $this->$property;
    }

    protected function getIncludes()
    {
        if ('s' == $_REQUEST['include']) {
            return <<<PHP
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
\n
PHP;
        }
        return '';
    }

    protected function getComentarioTopo()
    {
        $usunome = $_SESSION['usunome'];
        $usuemail = $_SESSION['usuemail'];
        $data = date('d-m-Y');

        list($dia, $mes, $ano) = explode('-', $data);

        return <<<PHP
<?php
/**
 * Classe de mapeamento da entidade {$this->schema}.{$this->tabela}.
 *
 * @version \$Id\$
 * @since {$ano}.{$mes}.{$dia}
 */
\n
PHP;
    }

    protected function getDockblockClasse()
    {
        $usunome = ucwords(strtolower($_SESSION['usunome']));
        $usuemail = $_SESSION['usuemail'];
        $data = date('d-m-Y');

        $nomeClasse = "{$this->prefixoClasse}_{$this->nomeClasse}";
        list($pacote1, $pacote2, $class) = explode('_', $nomeClasse);

        $propriedades = $this->getPropriedadesClasse();

        $descricaoTabela = !empty($this->atributos[0]['table_description'])
            ?$this->atributos[0]['table_description']
            :'sem descricao';

        return <<<PHP
/**
 * {$nomeClasse}: {$descricaoTabela}
 *
 * @package {$pacote1}\\{$pacote2}
 * @uses Simec\Db\Modelo
 * @author {$usunome} <{$usuemail}>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * \$model = new {$this->prefixoClasse}_{$this->nomeClasse}(\$valorID);
 * var_dump(\$model->getDados());
 *
 * // -- Alterando registros
 * \$valores = ['campo' => 'valor'];
 * \$model = new {$this->prefixoClasse}_{$this->nomeClasse}(\$valorID);
 * \$model->popularDadosObjeto(\$valores);
 * \$model->salvar(); // -- retorna true ou false
 * \$model->commit();
 * </code>
 *{$propriedades}
 */
class {$this->prefixoClasse}_{$this->nomeClasse} extends Modelo
{
PHP;
    }

    /**
     * Cria a lista de propriedades mágicas da classe.
     *
     * @return string
     * @todo Definir valores default das propriedades.
     */
    protected function getPropriedadesClasse()
    {
        $propriedade = <<<DOCBLOCK
\n * @property %s \$%s %s%s
DOCBLOCK;
        $retorno = '';

        foreach ($this->atributos as $atributo) {
            $retorno .= sprintf(
                $propriedade,
                $this->tipoPHP($atributo),
                $atributo['column_name'],
                $atributo['description'],
                !empty($atributo['column_default'])?" - default: {$atributo['column_default']}":''
            );
        }
        return $retorno;
    }

    protected function getAtributos($pkData, $fkData)
    {
        $stringClass = <<<PHP

    /**
     * @var string Nome da tabela mapeada.
     */
    protected \$stNomeTabela = '{$this->schema}.{$this->tabela}';

    /**
     * @var string[] Chave primaria.
     */
    protected \$arChavePrimaria = array(\n
PHP;
        foreach ($pkData ? $pkData : array() as $pk) {
            $stringClass .= <<<PHP
        '{$pk['column_name']}',\n
PHP;
        }
        $stringClass .= <<<PHP
    );
\n
PHP;

        $stringClass .= <<<PHP
    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected \$arChaveEstrangeira = array(\n
PHP;
        foreach ($fkData ? $fkData : array() as $fk) {
            $matches = array();
            $pattern = "/FOREIGN KEY \(([\w, ]+)\) REFERENCES ([\w_\.]+)\(([\w, ]+)\)/";
            preg_match($pattern, $fk['condef'], $matches);
            list(, $fk, $tabela, $pk) = $matches;

            $stringClass .= <<<PHP
        '{$fk}' => array('tabela' => '{$tabela}', 'pk' => '{$pk}'),\n
PHP;
        }

        $stringClass .= <<<PHP
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected \$arAtributos = array(\n
PHP;
        foreach ($this->atributos as $srAtributo) {
            $stringClass .= <<<PHP
        '{$srAtributo['column_name']}' => null,\n
PHP;
        }
        $stringClass .= <<<PHP
    );
\n
PHP;
        return $stringClass;
    }

    protected function getValidacao()
    {
        $stringClass = <<<PHP
    /**
     * Validators.
     *
     * @param mixed[] \$dados
     * @return mixed[]
     */
    public function getCamposValidacao(\$dados = array())
    {
        return array(\n
PHP;
        foreach ($this->atributos as $atributo) {

            $strValidacao = $this->getValidacaoCampo($atributo);

            $stringClass .= <<<PHP
            '{$atributo['column_name']}' => $strValidacao,\n
PHP;
        }
        $stringClass .= <<<PHP
        );
    }
\n
PHP;
        return $stringClass;
    }

    private function getValidacaoCampo($atributo)
    {
        $validacao = array();

        if ($atributo['is_nullable'] == 'YES') {
            $validacao[] = "'allowEmpty' => true";
        }

        switch ($atributo['data_type']) {
            case 'integer':
                $validacao[] = "'Digits'";
                break;
            case 'character varying':
            case 'character':
                $validacao[] = "new Zend_Validate_StringLength(array('max' => {$atributo['character_maximum_length']}))";
                break;
            case 'text':
                break;
            case 'date':
                //$validacao[] = " new Zend_Validate_Date() ";
                break;
            case 'boolean':
                break;
        }
        $str = implode(',', $validacao);
        return "array($str)";
    }

    /**
     * Converte o tipo do banco para um tipo PHP válido.
     *
     * @param string[] $atributo
     * @return string
     */
    private function tipoPHP($atributo)
    {
        switch ($atributo['data_type']) {
            case 'integer':
                return 'int';
            case 'character varying':
            case 'character':
            case 'text':
                return 'string';
            case 'boolean':
                return 'bool';
            case 'date':
            case 'timestamp with time zone':
            case 'timestamp without time zone':
                return '\\Datetime(Y-m-d H:i:s)';
            default:
                return $atributo['data_type'];
        }
    }

    public function gerarModel($pkData, $fkData)
    {
        $this->nomeClasse = ucFirst($this->tabela);
        $dirSchema =  APPRAIZ . "www/gerador/arquivos_gerados/{$this->schema}";
        $dir =  APPRAIZ . "www/gerador/arquivos_gerados/{$this->schema}/model";

        if(!is_dir($dirSchema)){
            mkdir($dirSchema, 0777);
        }
        if(!is_dir($dir)){
            mkdir($dir, 0777);
        }

        if (!$arquivo = fopen($dir."/{$this->nomeClasse}{$this->extensao}", "w+")) {
            return false;
        }

        $classe = $this->getModel($pkData, $fkData);

        if (!fwrite($arquivo, $classe)) {
            echo "Erro ao escrever no arquivo";
        } else {
            echo "Classe Model <b>{$this->tabela}</b> criada com sucesso.<br>";
        }
        fclose($arquivo);
    }

    public function getModel($pkData, $fkData)
    {
        $stringClass = $this->getComentarioTopo();
        $stringClass .= $this->getIncludes();
        $stringClass .= $this->getDockblockClasse();
        $stringClass .= $this->getAtributos($pkData, $fkData);
        $stringClass .= $this->getValidacao();

        $stringClass .=  <<<PHP
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

}

PHP;
        return $stringClass;
    }
}