<?php

/**
 * Classe de mapeamento da entidade emenda.emenda
 *
 * @version $Id: Solicitacoes.class.inc 121316 2017-04-04 18:53:04Z victorbarreto $
 */

/**
 * Emenda_Model_Emenda
 *
 * @package Spoemendas\Model\Emenda
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Spoemendas_Model_Solicitacoes extends Modelo
{

    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela = 'spoemendas.solicitacoes';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'solid',
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
        'solid' => null,
        'usucpf' => null,
        'solconteudo' => null,
        'docid' => null,
        'soltipo' => null,
        'soldata' => null,
        'autid' => null,
        'solassunto' => null,
        'soltipoalteracao' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'solconteudo' => array('allowEmpty' => false),
            'solassunto' => array('allowEmpty' => false),
            'autid' => array('allowEmpty' => false)
        );
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function retornaWhereAutorSolicitacao()
    {
        global $db;
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);
        if (in_array(PFL_PARLAMENTAR, $perfis)) {
            $sql = "SELECT autid FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'";
            $pfl = $db->carregar($sql);
            if (is_array($pfl)) {
                foreach ($pfl as $key => $value) {
                    $usuresp[$key] = $value['autid'];
                }
            }
            if (is_array($usuresp)) {
                $usurespIn = implode(',', $usuresp);
                $whereParlamentar = " AND aut.autid IN ($usurespIn)";
                $where .= $whereParlamentar;
            } else {
                $whereParlamentar = " AND false ";
                $where .= $whereParlamentar;
                $fm->addMensagem('Seu perfil de Parlamentar não está vinculado a nenhum autor. <br/><br/> Solicite a vinculação a ASPAR do MEC (2022-7899/7896/7894)', Simec_Helper_FlashMessage::ERRO);
            }
            return $where;
        }
    }

    /*
     * Valida se uma solicitação de informação ou alteração pode ser salva
     */

    public function podeSalvarSolicitacao($solid)
    {
        if (isset($solid) && (trim($solid) != '') && !$solid) {
            $perfis = pegaPerfilGeral($_SESSION['usucpf']);
            global $db;
            $sql = "SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid from spoemendas.solicitacoes WHERE solid = {$solid})";
            $esdid = $db->pegaUm($sql);
            /* Regra 1 - A solicitação só pode ser alterada se estiver nos
              seguintes estados do WF:
              - Em Preenchimento
              - Aguardado ajustes da UO
              - Aguardando ajustes da ASPAR (CGF)
              - Aguardando ajustes da ASPAR (CGO)
             */
            $statusLiberados = array(1751, 1753, 1752, 1745);
            /* Perfil Parlamentares */
            if (in_array(PFL_PARLAMENTAR, $perfis)) {
                if (in_array($esdid, $statusLiberados)) {
                    return true;
                }
                return false;
            }
            /* Perfil UO Equipe Técnica */
            if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
                if (in_array($esdid, $statusLiberados)) {
                    return true;
                }
                return false;
            }
            return true;
        } else {
            return true;
        }
    }
}
