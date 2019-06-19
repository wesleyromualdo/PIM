<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class ViewGenerator
{
    public $schema;
    public $prefixoClasse;
    public $tabela;
    public $atributos;
    public $nomeClasseModel;
    public $tabelaUpper1;

    private $_data;

    public function __construct (Array $properties = array())
    {
        if (!empty($properties)) {
            foreach ($properties as $key => $value) {
                $this->{$key} = $value;
            }
        }
        $this->_data = $properties;
    }

    public function setAtributos (Array $atributos = array())
    {
        if (!empty($atributos)) {
            foreach ($atributos as $key => $value) {
                $this->{$key} = $value;
            }
        }
    }

    public function __set ($property, $value)
    {
        return $this->_data[$property] = $value;
    }

    public function __get ($property)
    {
        return array_key_exists($property, $this->_data) ? $this->_data[$property] : $this->$property;
    }

    protected function getComentarioTopo()
    {
        $usunome = $_SESSION['usunome'];
        $usuemail = $_SESSION['usuemail'];
        $data = date('d-m-Y');
        return <<<PHP
<?php
/**
 * View da representando a tabela {$this->tabela}
 *
 * @category visao
 * @package  A1
 * @version \$Id\$
 * @author   {$usunome} <{$usuemail}>
 * @license  GNU simec.mec.gov.br
 * @version  Release: {$data}
 * @link     no link
 */\n
PHP;
    }

    protected function inicializeView ($pkData)
    {
        return <<<PHP
\$controller{$this->tabelaUpper1}     = new {$this->prefixoClasse}_Controller_{$this->tabelaUpper1}();

switch (\$_POST['acao']) {
	case 'salvar':
	    \$controller{$this->tabelaUpper1}->salvar(\$_POST);
	    break;
	case 'inativar':
	    \$id = \$_GET['{$pkData[0]['column_name']}'];
	    \$controller{$this->tabelaUpper1}->inativar(\$id);
	    break;
}
?>
PHP;
    }

    protected function getForm ($pkData, $fkData)
    {
        $stringClass = <<<PHP

<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo \$simec->hidden('acao', 'acao', ''); ?>

PHP;
        foreach ($pkData as $pk) {
            $stringClass .= <<<PHP
\n    <input type="hidden" name="{$pk['column_name']}" id="{$pk['column_name']}" value="<?php echo \$_GET['{$pk['column_name']}'] ?>"/>
PHP;
        }
        $stringClass .= <<<PHP
\n          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) {$this->tabelaUpper1}(a)</h3>
                </div>
                <div class="ibox-content">
PHP;
        $pks = [];
        foreach ($pkData as $atributo) {
            $pks[] = $atributo['column_name'];
        }
        $strCampos = $this->getCampos($pks, $fkData);
        $stringClass .= <<<PHP
\n              <?php $strCampos
                ?>
PHP;

        $stringClass .= <<<PHP
\n              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
                    </div>
                </div>
 \n           </div>
</form>
PHP;
        return $stringClass;
    }

    protected function getCampos ($pkData, $fkData)
    {
        $stringClass = '';
        foreach ($this->atributos as $atributo) {
            if (!in_array($atributo['column_name'], $pkData)) {
                $strCampos = $this->getCampo($atributo);
                $stringClass .= <<<PHP
    \n $strCampos
PHP;
            }
        }
        return $stringClass;
    }

    private function getCampo ($atributo)
    {
        $strColumnName = $atributo['column_name'];
        $strColumnNameComApas = "'{$atributo['column_name']}'";

        $attribs = array();
        $config = '';
        if ($atributo['is_nullable'] == 'YES') {
            $attribs[] = "'required'=>'required'";
        }

        if (!empty($atributo['character_maximum_length'])) {
            $attribs[] = "'maxlength'=>'{$atributo['character_maximum_length']}'";
        }

        $tipo = 'input';
        switch ($atributo['data_type']) {
            case 'integer':
                $attribs[] = "'class' => 'inteiro'";
                break;
            case 'text':
                $tipo = 'textArea';
                break;
            case 'character varying':
            case 'character':
                switch ($strColumnName) {
                    case strpos($strColumnName, 'cep') !== false :
                        $tipo = 'cep';
                        break;
                    case strpos($strColumnName, 'email') !== false :
                        $tipo = 'email';
                        break;
                    case strpos($strColumnName, 'cpf') !== false :
                        $tipo = 'cpf';
                        break;
                    case strpos($strColumnName, 'cnpj') !== false :
                        $tipo = 'cnpj';
                        break;
                }
                if ($atributo['character_maximum_length'] >= 500) {
                    $tipo = 'textArea';
                }

                break;
            case 'date':
                $tipo = 'data';
                break;
            case 'boolean':
                $tipo = 'radio';
                $config = ", array('type' => 'radio radio-info radio', 'style' => 'inline') ";
                break;
        }

        if (!empty($attribs)) {
            $attribs = ', array(' . implode(', ', $attribs) . ')';
        } else {
            $attribs = '';
        }
        return  <<<PHP
                    echo \$simec->{$tipo}({$strColumnNameComApas}, {$strColumnNameComApas}, \$obj{$this->tabelaUpper1}->{$strColumnName} {$attribs} $config);
PHP;
    }
    public function gerarView ($pkData, $fkData)
    {
        $this->nomeClasse = strtolower(str_replace('_', '', $this->tabela));
        $this->tabelaUpper1 = ucFirst($this->tabela);

        $dirSchema = APPRAIZ . "www/gerador/arquivos_gerados/{$this->schema}/";
        $dir = APPRAIZ . "www/gerador/arquivos_gerados/{$this->schema}/view/";

        if (!is_dir($dirSchema)) {
            mkdir($dirSchema, 0777);
        }
        if (!is_dir($dir)) {
            mkdir($dir, 0777);
        }


        if (!$arquivo = fopen($dir . "{$this->nomeClasse}{$this->extensao}", "w+")) {
            return false;
        }
        $classe = $this->getView($pkData, $fkData);


        if (!fwrite($arquivo, $classe)) {
            echo "Erro ao escrever no arquivo";
            ver($dir . "{$this->nomeClasse}{$this->extensao}", d);
        } else {
//            ver($dir ."{$this->nomeClasse}{$this->extensao}", d);
            echo "View <b>{$this->tabela}</b> criada com sucesso.<br>";
        }

        fclose($arquivo);
    }

    public function getView ($pkData, $fkData)
    {
        $stringClass = $this->getComentarioTopo();
        $stringClass .= $this->inicializeView($pkData);
        $stringClass .= $this->getForm($pkData, $fkData);

        return $stringClass;
    }
}