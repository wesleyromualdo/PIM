<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace Simec;

/**
 * Description of AbstractColecao
 *
 * @author calixto
 */
class AbstractColecao extends \MecCoder\AbstractCollection implements \JsonSerializable
{

    /**
     * Caso não exista o método na coleção ela chama nos itens
     * - em todos os itens que possuirem o método
     * @param type $name
     * @param type $arguments
     */
    public function __call($name, $arguments)
    {
        foreach ($this as $idx => $item) {
            $refl = new \ReflectionClass($item);
            if ($refl->hasMethod($name) && $refl->getMethod($name)->isPublic()) {
                $refl->getMethod($name)->invokeArgs($item, $arguments);
            }
        }
        return $this;
    }

    /**
     * Executa uma função em todos os elementos da coleção
     * @param function $funcao
     */
    public function executar($funcao)
    {
        if (is_callable($funcao)) {
            foreach ($this as $idx => $item) {
                $funcao($this[$idx], $idx);
            }
        }
        return $this;
    }

    /**
     * Cria uma coleção com um subconjuntos de dados com caracteristicas iguais
     * @param type $atributo
     * @param type $valor
     * @param type $comparacao
     * @throws Excecao\Colecao
     */
    public function subColecao($atributo, $valor, $comparacao = null)
    {
        $comparacao = is_callable($comparacao) ? $comparacao : function($item) use ($atributo, $valor) {
            return $item->{$atributo} == $valor;
        };
        $classe = (new \ReflectionClass($this))->getName();
        $colecao = new $classe();
        $primeiro = $this->primeiro();
        $reflectionTipo = new \ReflectionClass($primeiro);
        $tipo = $reflectionTipo->getName();
        if (!$reflectionTipo->hasProperty($atributo)) {
            throw new Excecao\Colecao(sprintf(
                'Erro ao gerar subcoleção de %s, ' .
                'o atributo %s é inexistente na classe %s'
                , $classe, $atributo, $tipo));
        }
        foreach ($this as $idx => $item) {
            if ($comparacao($item)) {
                $colecao[] = clone $item;
            }
        }
        return $colecao;
    }

    public function limpar()
    {
        foreach ($this as $idx => $item) {
            $this->offsetUnset($idx);
        }
    }

    /**
     * Recria a coleção indexando pelo valor de um atributo
     * @param string $atributo
     */
    public function indexar($atributo)
    {
        $reflection = new \ReflectionClass($this->primeiro());
        $res = [];
        $res = $this->getArrayCopy();
        $this->limpar();
        foreach ($res as $idx => $item) {
            $this->offsetSet($item->{$atributo}, $item);
        }
        return $this;
    }

    /**
     * Retorna o primeiro item da coleção
     * @return AbstractDado
     */
    public function primeiro()
    {
        return array_shift($this->getArrayCopy());
    }

    /**
     * Retorna o último item da coleção
     * @return AbstractDado
     */
    public function ultimo()
    {
        return array_pop($this->getArrayCopy());
    }

    /**
     * Retorna se a coleção está preenchida
     * @return boolean
     */
    public function possuiItens()
    {
        return $this->count() ? true : false;
    }

    /**
     * Define um item na coleção pelo indice
     * @param type $index
     * @param \Simec\AbstractDado $newval
     * @throws Excecao\Colecao
     */
    public function offsetSet($index, $newval)
    {
        if (!($newval instanceof AbstractDado)) {
            throw new Excecao\Colecao('Tipo de dado inválido, a coleção somente aceita um "AbstractDados"');
        }
        parent::offsetSet($index, $newval);
    }
    
    /**
     * Retorna um elemento da coleção
     * @param type $index
     * @return AbstractDado
     */
    public function offsetGet($index)
    {
        return parent::offsetGet($index);
    }
    
    public function jsonSerialize(){
        return (array) $this;
    }
}
