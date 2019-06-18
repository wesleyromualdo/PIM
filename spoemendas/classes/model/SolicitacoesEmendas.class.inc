<?php

/**
 * Classe de mapeamento da entidade emenda.emenda
 *
 * @version $Id: SolicitacoesEmendas.class.inc 112058 2016-07-01 14:30:50Z werteralmeida $
 */

/**
 * Emenda_Model_Emenda
 *
 * @package Spoemendas\Model\Emenda
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Spoemendas_Model_SolicitacoesEmendas extends Modelo {

    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela = 'spoemendas.solicitacoesemendas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'soeid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos
     */
    protected $arAtributos = array(
        'soeid' => null,
        'solid' => null,
        'emeid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array()) {
        return array(
        );
    }

    public function limparPorSolid($solid) {
        global $db;
        $delete = " DELETE FROM spoemendas.solicitacoesemendas WHERE solid = {$solid}";
        $db->executar($delete);
        $db->commit();
    }

    public function carregarEmendasCombo($solid) {
        global $db;
        if ($solid) {
            $sql = "SELECT emeid FROM spoemendas.solicitacoesemendas WHERE solid = {$solid}";
            $dados = $db->carregar($sql);
            if (is_array($dados)) {
                foreach ($dados as $valor) {
                    $saida[] = $valor['emeid'];
                }
            }
            return $saida;
        }
    }

}
