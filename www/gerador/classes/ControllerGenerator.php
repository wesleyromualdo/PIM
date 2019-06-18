<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class ControllerGenerator
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

    protected function getComentarioTopo()
    {
        $usunome = $_SESSION['usunome'];
        $usuemail = $_SESSION['usuemail'];
        $data = date('d-m-Y');
        return <<<PHP
<?php
/**
 * Classe de controle do  {$this->tabela}
 * @category Class
 * @package  A1
 * @version \$Id\$
 * @author   {$usunome} <{$usuemail}>
 * @license  GNU simec.mec.gov.br
 * @version  Release: {$data}
 * @link     no link
 */

\n
PHP;
    }

    protected function getNomeClasse($pkData)
    {
        $data = date('d-m-Y');
        $pk = ( !empty($pkData) ? $pkData[0]['column_name'] : '');

        return <<<PHP

/**
 * {$this->prefixoClasse}Controller_{$this->nomeClasse}
 *
 * @category Class
 * @package  A1
 * @author   {$usunome} <{$usuemail}>
 * @license  GNU simec.mec.gov.br
 * @version  Release: {$data}
 * @link     no link
 */
class {$this->prefixoClasse}Controller_{$this->nomeClasse}
{
    private \$model;

    public function __construct()
    {
        \$this->model = new {$this->prefixoClasse}Model_{$this->nomeClasse}(\$_GET['{$pk}']);
    }

PHP;
    }

    protected function getSalvar($pkData)
    {
        $pk = ( !empty($pkData) ? $pkData[0]['column_name'] : '');
        $classeVarModel  = 'this->model';//strtolower($this->nomeClasse);
        return <<<PHP
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global \$url;
//        \${$classeVarModel} = new {$this->prefixoClasse}Controller_{$this->nomeClasse}();
        \${$classeVarModel}->popularDadosObjeto();
        \$url .= '&{$pk}=' . \${$classeVarModel}->{$pk};

        try{
            \$sucesso = \${$classeVarModel}->salvar();
            \${$classeVarModel}->commit();
        } catch (Simec_Db_Exception \$e) {
            simec_redirecionar(\$url, 'error');
        }

        if(\$sucesso){
            simec_redirecionar(\$url, 'success');
        }
        simec_redirecionar(\$url, 'error');
    }\n
\n
PHP;
    }

    protected function getInativar($pkData)
    {
        $pk = ( !empty($pkData) ? $pkData[0]['column_name'] : 'id');
        $classeVarModel = strtolower($this->nomeClasse);
        return <<<PHP
   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global \$url;
        \$id = \$_GET['{$pk}'];
        \$url = "aspar.php?modulo=principal/proposicao/index&acao=A&{$pk}={\$id}";
        \${$classeVarModel} = new {$this->prefixoClasse}Model_{$this->nomeClasse}(\$id);
        try{
             \${$classeVarModel}->Xstatus = 'I';
             \${$classeVarModel}->Xdtinativacao = date('Y-m-d H:i:s');
             \${$classeVarModel}->Xusucpfinativacao = \$_SESSION['usucpf'];

             \${$classeVarModel}->salvar();
             \${$classeVarModel}->commit();
            simec_redirecionar(\$url, 'success');
        } catch (Simec_Db_Exception \$e) {
            simec_redirecionar(\$url, 'error');
        }
    }\n
\n
PHP;
    }

    public function gerarController($pkData, $fkData)
    {
        $this->nomeClasse = ucFirst($this->tabela);
        $dirSchema =  APPRAIZ . "www/gerador/arquivos_gerados/{$this->schema}";
        if(!is_dir($dirSchema)){
            mkdir($dirSchema, 0777);
        }

        $dir =  APPRAIZ . "www/gerador/arquivos_gerados/{$this->schema}/controller";
        if(!is_dir($dir)){
            mkdir($dir, 0777);
        }

        if (!$arquivo = fopen($dir."/{$this->nomeClasse}{$this->extensao}", "w+")) {
            return false;
        }

        $classe = $this->getController($pkData);


        if (!fwrite($arquivo, $classe)) {
            echo "Erro ao escrever no arquivo";
        } else {
            echo "Classe Controller <b>{$this->tabela}</b> criada com sucesso.<br>";
        }
        fclose($arquivo);
    }

    public function getController($pkData)
    {
        $stringClass = $this->getComentarioTopo();
        $stringClass .= $this->getNomeClasse($pkData);
        $stringClass .= $this->getSalvar($pkData);
        $stringClass .= $this->getInativar($pkData);

        $stringClass .=  <<<PHP
\n}
?>
PHP;
        return $stringClass;
    }
}