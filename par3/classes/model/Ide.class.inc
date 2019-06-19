<?php
/**
 * Classe de mapeamento da entidade ide2016.tb_ide_tabela11a_uf_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-12-2015
 * @link     no link
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Tb_ide_tabela11a_uf_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-12-2015
 * @link     no link
 */
class Par3_Model_Ide extends Modelo
{

    protected $arAtributos = array(
        'tipo' => null,
        'estuf' => null,
        'muncod' => null,
    );

    public function getTituloEstado()
    {
        if (empty($this->muncod)) {
            $estado = $this->pegaUm(" select estdescricao from territorios.estado where estuf = '{$this->estuf}' ");
            echo " - <b style='color: #F9C301'> {$estado}</b>";
        }
    }

    public function getTituloMunicipio()
    {
        if (!empty($this->muncod)) {
            $municipio = $this->pegaUm(" SELECT mundescricao || ' - ' || estuf  FROM territorios.municipio WHERE muncod = '{$this->muncod}' ");
            echo " - <b style='color: #F9C301'> {$municipio}</b>";
        }
    }

    public function getEstados()
    {
        return "select estuf as codigo, estuf || ' - ' || estdescricao as descricao from territorios.estado order by estuf";
    }

    public function getMunicipio($muncod)
    {
        $sql = "select mundescricao as descricao
                from territorios.municipio
                where muncod = '$muncod' limit 1";
        return $this->pegaUm($sql);
    }

    public function montarMunicipios($estuf = null, $muncod = null)
    {
        global $simec;
        $sql = "select muncod as codigo, estuf || ' - ' || mundescricao as descricao
                from territorios.municipio
                where estuf = '$estuf'
                order by estuf, mundescricao";

        return $simec->select('muncod', 'Município', $muncod, $sql, array('required'));
    }

    public function getTitulo($rede, $tabelaUf, $tabelaMu)
    {
        if ($this->tipo == 'estado') {
            $tabela = "Par3_Model_Ide_" . $tabelaUf;
            $tbIdeTabela = new $tabela;
            echo $tbIdeTabela->getTitulo($rede, $this->estuf);
        } elseif ($this->tipo == 'municipio') {
        	if(!empty($tabelaMu)){
            	$tabela = "Par3_Model_Ide_" . $tabelaMu;
            	$tbIdeTabela = new $tabela;
            	echo $tbIdeTabela->getTitulo($rede, $this->muncod);
        	}
        }
    }

    public function getTabela($rede, $tabelaUf, $tabelaMu)
    {
        if ($this->tipo == 'estado') {
            $tabela = "Par3_Model_Ide_" . $tabelaUf;
            $tbIdeTabela = new $tabela;
            $tbIdeTabela->getTabela($rede, $this->estuf);
        } elseif ($this->tipo == 'municipio') {
        	if(!empty($tabelaMu)){
	            $tabela = "Par3_Model_Ide_" . $tabelaMu;
	            $tbIdeTabela = new $tabela;
	            $tbIdeTabela->getTabela($rede, $this->estuf, $this->muncod);
        	}
        }
    }
}//end Class
?>