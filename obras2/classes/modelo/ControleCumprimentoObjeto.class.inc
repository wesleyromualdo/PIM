<?php

/**
 * Description of CumprimentoObjeto
 *
 * @author lindalbertofilho
 */
class ControleCumprimentoObjeto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.controle_cumprimento_objeto";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ccoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ccoid' => null,
        'obrid' => null,
        'dtcriacao' => null,
        'ccostatus' => null
    );

    public function verificaObra($obrid)
    {
        $sql    = "SELECT obrid FROM obras2.controle_cumprimento_objeto WHERE ccostatus = 'A' AND obrid = $obrid";
        $result = $this->pegaUm($sql);
        return ($result == $obrid) ? true : false;
    }

    public function inserirObra($obrid)
    {
        $this->popularDadosObjeto(array('obrid' => $obrid));
        $this->salvar();
        $this->commit();
        return $this->ccoid;
    }

    public function removerObra($obrid)
    {
        $sql    = "UPDATE obras2.controle_cumprimento_objeto SET ccostatus = 'I' WHERE ccostatus = 'A' AND obrid = $obrid";
        return $this->executar($sql);
    }

}