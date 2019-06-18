<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace Simec;

/**
 * Este objeto serve para definir a estrutura e as regras de um escopo de dados,
 * como por exemplo de uma tabela do banco
 *
 * @author calixto
 */
abstract class AbstractDado extends \MecCoder\AbstractValueObject implements \JsonSerializable
{
    const nulo = '«ØØØ»';

    public function __construct(array $ar = null)
    {
        $this->carregar($ar);
    }

    /**
     * Executa o mapeamento da documentação de um AbstractDado
     * @return array
     * @throws Excecao\Dado
     */
    public function mapear()
    {
        $reflexao = new \ReflectionClass($this);
        $atributos = $reflexao->getProperties();
        $ar = [];
        foreach ($atributos as $atributo) {
            $arComentario = explode("\n", $atributo->getDocComment());
            $campo = [
                'atributo' => null,
                'campo' => null,
                'valor' => null,
                'tipo' => null
            ];
            if ($arComentario) {
                foreach ($arComentario as $parte) {
                    if (preg_match('/\*\s+\@(.*)\s+(.*)/', $parte, $match)) {
                        if (isset($match[1])) {
                            if ($match[1] == 'tipo') {
                                $campo['tipo'] = $match[2];
                            }
                            if ($match[1] == 'chave') {
                                $campo['chave'] = $match[2];
                            }
                            if ($match[1] == 'nome') {
                                $campo['nome'] = $match[2];
                            }
                            if ($match[1] == 'nomeAbreviado') {
                                $campo['nomeAbreviado'] = $match[2];
                            }
                            if ($match[1] == 'campo') {
                                $campo['atributo'] = $atributo->getName();
                                $campo['valor'] = $atributo->getValue($this);
                                $campo['campo'] = $match[2];
                            }
                        }
                    }
                }
            }
            if ($campo['campo']) {
                $ar['campos'][] = $campo;
            }
        }
        $arComentario = explode("\n", $reflexao->getDocComment());
        if ($arComentario) {
            foreach ($arComentario as $parte) {
                if (preg_match('/\*\s+\@(.*)\s+(.*)/', $parte, $match)) {
                    if (isset($match[1])) {
                        if ($match[1] == 'tabela') {
                            $ar['tabela'] = $match[2];
                        }
                    }
                }
            }
        }
        if (!isset($ar['tabela'])) {
            throw new Excecao\Dado('Doc @tabela : O nome da tabela não foi definido na classe');
        }
        if (!isset($ar['campos'])) {
            throw new Excecao\Dado('Doc @campo : Não foi mapeado nenhum campo na classe');
        }

        return $ar;
    }

    public function autoValidar()
    {
        $validarCampo = function($campo) {
            //rotear para validação
        };
        $reflexao = new \ReflectionClass($this);
        $atributos = $reflexao->getProperties();
        $ar = [];
        foreach ($atributos as $atributo) {
            $metodo = 'validar' . ucFirst($atributo->getName());
            if ($reflexao->hasMethod($metodo)) {
                $fn = $reflexao->getMethod($metodo);
                $fn->invoke($reflexao);
            }
            $arComentario = explode("\n", $atributo->getDocComment());
            foreach ($arComentario as $parte) {
                $campo = [];
                if (preg_match('/\*\s+\@(.*)\s+(.*)/', $parte, $match)) {
                    foreach(['nome','tipo','tamanho','obrigatorio'] as $tipo){
                        if ($match[1] == $tipo) {
                            $campo[$tipo] = $match[2];
                        }
                    }
                    $metodo = 'validarPorTipo' . ucFirst($campo['tipo']);
                    if ($reflexao->hasMethod($metodo)) {
                        $fn = $reflexao->getMethod($metodo);
                        $fn->invoke($reflexao);
                    }
                    $campo['valor'] = $atributo->getValue();
                    $validarCampo($campo);
                }
            }
        }
    }

    /**
     * Carrega o objeto com um array
     * @param array $ar
     */
    public function carregar($ar)
    {
        if ($ar) {
            $class = new \ReflectionClass($this);
            foreach ($ar as $atributo => $valor) {
                if ($class->hasProperty($atributo)) {
                    $this->{$atributo} = $valor;
                }
            }
        }
        return $this;
    }

    public function enviarBanco()
    {
        return $this;
    }

    public function enviarControle()
    {
        return $this;
    }

    public function enviarVisao()
    {
        return $this;
    }
    
    public function jsonSerialize()
    {
        $vars = get_object_vars($this);
        foreach($vars as $idx => $val){
            $vars[$idx] = ($val);
        }
        return $vars;
    }
    
}
