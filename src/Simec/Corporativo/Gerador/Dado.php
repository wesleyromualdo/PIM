<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace Simec\Corporativo\Gerador;

/**
 * Description of Dado
 *
 * @author calixto
 */
class Dado
{

    public function gerar($esquema, $tabela, $namespace = null)
    {
        $colTabela = (new \Simec\Corporativo\Modelo\EstruturaTabela())->lerDefinicaoTabela($esquema, $tabela);
        $colCampos = (new \Simec\Corporativo\Modelo\EstruturaTabela())->lerDefinicaoCampos($esquema, $tabela);
        
        $campos = '';
        foreach($colCampos as $campo){
            $campos.= $this->gerarCodigoCampo($campo);
        }
        echo sprintf($this->gerarCodigoTabela($colTabela->primeiro(),$namespace),$campos);
    }
    

    /**
     * Realiza a documentação do campo
     */
    public function gerarCodigoTabela($eTabela, $namespace = null)
    {
        $Nome = ucfirst($eTabela->nome);
        $Esquema = ucfirst($eTabela->esquema);
        if(!$namespace){
            $namespace = "Simec\\{$Esquema}\\Dado";
        }else{
            $namespace = str_replace('\\','\\\\', $namespace);
        }
        return <<<doc
<?php
namespace {$namespace};

/**
 * Estrutura de dados referente à: {$eTabela->esquema}.{$eTabela->nome}
 * @description {$eTabela->descricao}  
 * @tabela {$eTabela->esquema}.{$eTabela->nome}
 */
class {$Nome} extends \Simec\AbstractDado
{\n
%s
}
doc;
    }    
    
    /**
     * Realiza a documentação do campo
     */
    public function gerarCodigoCampo($eCampo)
    {
        $chave = $eCampo->campoPk ? "\n     * @chave true" : '';
        $obrigatorio = $eCampo->obrigatorio ? "\n     * @obrigatorio true" : '';
        switch ($eCampo->tipoDado) {
            case 'texto':
                $var = "\n     * @var string";
                break;
            case 'numerico':
                $var = "\n     * @var float";
                break;
            case 'data':
                $var = "\n     * @var string";
                break;
        }
        return sprintf(<<<doc
    /**
     * %s
     * @nome 
     * @nomeAbreviado
     * @campo %s
     * @tipo %s%s%s
     * @tamanho %s
     * @validador
     * @var string
     */
    public $%s;\n\n
doc
            , $eCampo->descricao, $eCampo->campo, $eCampo->tipoDado, $chave, $obrigatorio, $eCampo->tamanho, $eCampo->campo
        );
    }
}
