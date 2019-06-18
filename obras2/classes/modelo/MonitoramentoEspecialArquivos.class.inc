<?php

class MonitoramentoEspecialArquivos extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.monitoramento_especial_arquivos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("meaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'meaid' => null,
        'mesid' => null,
        'arqid' => null,
        'meastatus' => null,
    );

    public function pegaArquivosPorMesid($mesid)
    {
        $sql = "
            SELECT a.*, m.meaid FROM obras2.monitoramento_especial_arquivos m
            JOIN arquivo a ON a.arqid = m.arqid
            WHERE m.meastatus = 'A' AND mesid = $mesid
        ";
        return $this->carregar($sql);
    }
}