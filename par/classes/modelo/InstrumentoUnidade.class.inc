<?php

class InstrumentoUnidade extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.instrumentounidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("inuid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'inuid' => null,
        'itrid' => null,
        'inudata' => null,
        'estuf' => null,
        'muncod' => null,
        'mun_estuf' => null,
        'usucpf' => null,
    );

    public function verificaInuidMunicipio($muncod) {
        $sql = "select inuid from {$this->stNomeTabela} where muncod = '{$muncod}'";
        return $this->pegaUm($sql);
    }

    public function verificaInuidEstado($estuf) {
        $sql = "select inuid from {$this->stNomeTabela} where estuf = '{$estuf}'";
        return $this->pegaUm($sql);
    }

    /**
     * Retorna Lista de E-mail e Nome da Entidades por Municipio ou UF
     * @param type $arrPost
     * return array
     * @TODO Correcao de Tipo de envio de e-mail para termo
     */
    public function getMailByMunicipioUf($arrPost) {
        $where = '';
        if ($arrPost['esfera'] == 'M') {
            $arrMunicipio = implode("','", $arrPost['listaMunicipio']);
            $where = "iu.muncod in ('{$arrMunicipio}')";
        } else if ($arrPost['esfera'] == 'E') {
            $arrUf = implode("','", $arrPost['estuf']);
            $where = "iu.mun_estuf in ('{$arrUf}')";
        } else {
           return array();
        }

        # Buscar email para envio do municipio
        $sql = "
            SELECT 
                en.entnome,
                en.entemail
            FROM par.instrumentounidade iu 
            INNER JOIN par.entidade en ON iu.inuid=en.inuid
            WHERE {$where}
            AND en.dutid = 6
        ";
        return $this->carregar($sql);
    }


    /**
     * Função prepararFiltro
     * - monta o filtro SQL para lista de intstrumentos unidade.
     *
     * @return array com dados principais,
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     */
    public function getDadosUnidade($inuid)
    {
        $sql = "SELECT
			itrid,
			estuf,
			inudescricao,
			muncod
		FROM
			par.instrumentounidade
		WHERE
			inuid = {$inuid}";

        return $this->pegaLinha($sql);
    }

    public function getEntidadeInadimplente($inuid)
    {
//        $sql = "select contas from par3.vm_relatorio_quantitativo_pendencias where inuid = {$inuid}";
        $sql = "SELECT inuid_par3 FROM par.v_entidade_inadimplente_prestacaocontas WHERE inuid_par3 = {$inuid}";

        if ($this->pegaUm($sql)) {
            return 1;
        } else {
            return 0;
        }
    }
}
