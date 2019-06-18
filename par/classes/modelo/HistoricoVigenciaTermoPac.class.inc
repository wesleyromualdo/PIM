<?php

/**
 * Classe de mapeamento da entidade par.solicitacaoprorrogacaoprazoobra
 *
 * @category Class
 * @package  A1
 * @author   JAIR FERREIRA FORO SANTOS <jair.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */

/**
 * HistoricoVigenciaTermoPac
 *
 * @category Class
 * @package  A1
 * @author   JAIR FERREIRA FORO SANTOS <jair.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-05-2016
 * @link     no link
 */
class HistoricoVigenciaTermoPac extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par.historicovigenciatermopac';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'hvtid'
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'proid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hvtid' => null,
        'proid' => null,
        'usucpf' => null,
        'sppid' => null,
        'hvttipodeacao' => null,
        'hvtvigenciaanterior' => null,
        'hvtvigencianova' => null,
        'hvtdataacao' => null,
        'hvtjustificativa' => null,
    );

    /**
     * Insere Historico de Vigencia PAC
     * @param type $arrData
     * @return type
     */
    public function insert($arrData) {
        try {
            $this->popularDadosObjeto($arrData);
            $sppid = $this->salvar();
            return $sppid;
        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
        }
    }

}

//end Class
?>