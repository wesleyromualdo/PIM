<?php

class SolicitacaoDesembolsoConvenioArquivos extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_desembolso_convenio_arquivos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sdaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sdaid' => null,
        'sdcid' => null,
        'arqid' => null,
        'sdastatus' => null,
    );

    public function pegaArquivosPorSdcid($sdcid)
    {
        $sql = "
            SELECT a.*, m.sdaid FROM obras2.solicitacao_desembolso_convenio_arquivos m
            JOIN arquivo a ON a.arqid = m.arqid
            WHERE m.sdastatus = 'A' AND sdcid = $sdcid
        ";
        return $this->carregar($sql);
    }
}