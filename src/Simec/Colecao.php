<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace Simec;

/**
 * Description of Colecao
 *
 * @author calixto
 */
class Colecao extends AbstractColecao
{

    /**
     * Cria uma coleção com um subconjuntos de dados com caracteristicas iguais
     * @param string $atributo
     * @param mixed $valor
     * @param function $comparacao
     * @throws Excecao\Colecao
     * @return Colecao
     */
    public function subColecao($atributo, $valor, $comparacao = null)
    {
        return parent::subColecao($atributo, $valor, $comparacao);
    }

    /**
     * Realiza a indexação da coleção por um atributo
     * @param string $atributo
     */
    public function indexar($atributo)
    {
        return parent::indexar($atributo);
    }

    /**
     * Retorna o primeiro elemento da coleção
     * @return AbstractDados
     */
    public function primeiro()
    {
        return parent::primeiro();
    }

    /**
     * Retorna o último elemento da coleção
     * @return AbstractDados
     */
    public function ultimo()
    {
        return parent::ultimo();
    }
    
    
    
}
