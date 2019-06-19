<?php

/**
 * Controle da funcionalidade obra_estudo_demanda
 *
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 * @license GNU simec.mec.gov.br
 *N @version $Id$
 */
class Par3_Controller_ObraEstudoDemanda extends Controle
{
    /**
     * Variável para instância do modelo.
     *
     * @var object
     */
    protected $_modObra_estudo_demanda;

    /**
     * Variável para guardar a url da funcionalidade.
     *
     * @var string
     */
    protected $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraEstudoDemanda();
    }

    /**
     * Função responsável por salvar
     *
     * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
     * @filesource
     * @throws Exception Mensagem de erro
     * @param array $dados Dados de assunto
     * @return bool Resultado da transação
     */
    public function salvar(array $dados)
    {
        try {
            if (empty($dados)) {
                throw new Exception('Dados não informados');
            }

            $this->model->popularDadosObjeto($dados);
            $oedid = $this->model->salvar();
            $this->model->commit();
            return $oedid;
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($this->_url, 'error', $e->getMessage());
        }
    }

    public function salvarPasso($dados)
    {
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        $arrOed = array();
        //SALVAR PASSO 1 DO QUESTIONÁRIO
        $arrOed['oedid'] = $dados['oedid'];
        $arrOed['obrid'] = $dados['obrid'];
        $arrOed['omoid'] = $dados['omoid'];
        $arrOed['etaid'] = $dados['etaid'];
        //****************************************************************
        //SALVAR PASSO 1 DO QUESTIONARIO
        if($dados['passo'] == 1){
            if($arrOed['omoid'] != 3){
                $arrOed['oedquadra'] = 'N';
            }
            $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],1);
            return $this->salvar($arrOed);
        }
        //****************************************************************
        //SALVAR PASSO 2 DO QUESTIONÁRIO
        if($dados['passo'] == 2){
            if($dados['omoid'] == 3){
                $arrOed['oedquadra']                   = $dados['oedquadra'];
                $arrOed['edaid']                       = $dados['edaid'];
                $arrOed['oedtipoampliacao_outros_dsc'] = trim(($dados['oedtipoampliacao_outros_dsc']));
                $arrOed['oedtipoampliacao_outros']     = $dados['oedtipoampliacao_outros'];
                $arrOed['oemid']                       = $dados['oemid'];

                if($dados['edaid']){
                    $mEda = new Par3_Model_ObraEstudoDemandaXampliacao();
                    $mEda->salvarAmpliacoes($arrOed['oedid'], $arrOed['edaid']);
                }
                $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],9);
                return $this->salvar($arrOed);
            }

            if($dados['omoid'] == 2 || $dados['omoid'] == 1){
                $arrOed['oedturnos_atendimento']        = $dados['oedturnos_atendimento'];
                $arrOed['oednoturno']                   = $dados['oednoturno'];
                $arrOed['oemid']                        = $dados['oemid'];
                $arrOed['oedmodalidade_noturno_outros'] = trim(($dados['oedmodalidade_noturno_outros']));
                $arrOed['oedquadra']    = 'N';
                $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],2);
                return $this->salvar($arrOed);
            }
        }
        //****************************************************************
        //SALVAR PASSO 3 DO QUESTIONÁRIO
        if($dados['passo'] == 3) {
            if ($dados['omoid'] == 1 || $dados['omoid'] == 2) {
                if ($dados['etaid'] == 1) {
                    $arrOed['oedcreche_1_qtd'] = $dados['oedcreche_1_qtd'];
                    $arrOed['oedcreche_2_qtd'] = $dados['oedcreche_2_qtd'];
                    $arrOed['oedcreche_3_qtd'] = $dados['oedcreche_3_qtd'];
                    $arrOed['oedpre_escola_qtd'] = $dados['oedpre_escola_qtd'];
                    $arrOed['oedquadra']    = 'N';
                    $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],3);
                    return $this->salvar($arrOed);
                }
                if ($dados['etaid'] == 2) {
                    $arrOed['oed1_serie_qtd'] = $dados['oed1_serie_qtd'];
                    $arrOed['oed2_serie_qtd'] = $dados['oed2_serie_qtd'];
                    $arrOed['oed3_serie_qtd'] = $dados['oed3_serie_qtd'];
                    $arrOed['oed4_serie_qtd'] = $dados['oed4_serie_qtd'];
                    $arrOed['oed5_serie_qtd'] = $dados['oed5_serie_qtd'];
                    $arrOed['oed6_serie_qtd'] = $dados['oed6_serie_qtd'];
                    $arrOed['oed7_serie_qtd'] = $dados['oed7_serie_qtd'];
                    $arrOed['oed8_serie_qtd'] = $dados['oed8_serie_qtd'];
                    $arrOed['oed9_serie_qtd'] = $dados['oed9_serie_qtd'];
                    $arrOed['oedquadra']    = 'N';
                    $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],3);
                    return $this->salvar($arrOed);
                }
                if ($dados['etaid'] == 3) {
                    $arrOed['oed1_ano_qtd'] = $dados['oed1_ano_qtd'];
                    $arrOed['oed2_ano_qtd'] = $dados['oed2_ano_qtd'];
                    $arrOed['oed3_ano_qtd'] = $dados['oed3_ano_qtd'];
                    $arrOed['oedquadra']    = 'N';
                    return $this->salvar($arrOed);
                }
            }
            if ($dados['omoid'] == 3) {
                $arrOed['oedapliacao_justificativa'] = trim(($dados['oedapliacao_justificativa']));
                $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],5);
                return $this->salvar($arrOed);
            }
        }
        //****************************************************************
        //SALVAR PASSO 4 DO QUESTIONÁRIO
        if($dados['passo'] == 4) {
            if($dados['omoid'] == 1){
                $mEscola = new Par3_Model_ObraEstudoDemandaXescola();
                $arrOed['etaid'] = $dados['etaid'];
                $arrEscid  = $dados['escid'];
                $arrEscQtd = array();
                if($arrOed['etaid'] == 1){$arrEscQtd = $dados['ederemanejamento_infantil'];}
                if($arrOed['etaid'] == 2){$arrEscQtd = $dados['ederemanejamento_fundamental'];}
                if($arrOed['etaid'] == 3){$arrEscQtd = $dados['ederemanejamento_medio'];}
                $mEscola->salvarEscolas($dados['oedid'],$arrEscid,$arrEscQtd,$arrOed['etaid']);
                $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],5);
            }

            if ($dados['omoid'] == 2) {
                $arrOed['oedexpansao_justificativa_dsc'] = trim(($dados['oedexpansao_justificativa_dsc']));
                $arrOed['oedquadra']    = 'N';
                $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],6);
            }
            if($dados['omoid'] == 3){
            }
            return $this->salvar($arrOed);
        }

        if($dados['passo'] == 5){
            if($dados['omoid'] == 1) {
                $arrOed['oedremanejamento_justificativa']     = $dados['oedremanejamento_justificativa'];
                $arrOed['oedremanejamento_justificativa_dsc'] = trim(($dados['oedremanejamento_justificativa_dsc']));
                $arrOed['oedquadra']    = 'N';
                $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],7);
                return $this->salvar($arrOed);
            }
        }
    }

    public function arrAtributos()
    {
        return $arAtributos = array(
            'oedexpansao_justificativa' => null,
            'oedremanejamento_justificativa' => null,
            'oedapliacao_justificativa' => null,
            'oedquadra' => null,
            'oeddemanda_justificativa' => null,
            'oed3_ano_qtd' => null,
            'oed3_ano' => null,
            'oed2_ano_qtd' => null,
            'oed2_ano' => null,
            'oed1_ano_qtd' => null,
            'oed1_ano' => null,
            'oed9_serie_qtd' => null,
            'oed9_serie' => null,
            'oed8_serie_qtd' => null,
            'oed8_serie' => null,
            'oed7_serie_qtd' => null,
            'oed7_serie' => null,
            'oed6_serie_qtd' => null,
            'oed6_serie' => null,
            'oed5_serie_qtd' => null,
            'oed5_serie' => null,
            'oed4_serie_qtd' => null,
            'oed4_serie' => null,
            'oed3_serie_qtd' => null,
            'oed3_serie' => null,
            'oed2_serie_qtd' => null,
            'oed2_serie' => null,
            'oed1_serie_qtd' => null,
            'oed1_serie' => null,
            'oedpre_escola_qtd' => null,
            'oedpre_escola' => null,
            'oedcreche_3_qtd' => null,
            'oedcreche_3' => null,
            'oedcreche_2_qtd' => null,
            'oedcreche_2' => null,
            'oedcreche_1_qtd' => null,
            'oedcreche_1' => null,
            'oedtipoampliacao_outros' => null,
            'oedtipoampliacao_outros_dsc' => null,
            'oedmodalidade_noturno_outros' => null,
            'oedmodalidade_noturno' => null,
            'oednoturno' => null,
            'oedturnos_atendimento' => null,
            'oedqtd_salas' => null,
            'omoid' => null,
            'obrid' => null,
            'oedid' => null,
            'oemid' => null,
            'etaid' => null,
            'oedexpansao_justificativa_dsc' => null,
            'oedremanejamento_justificativa_dsc' => null,
        );
    }

    /**
     * Método responsável por recuperar os dados apartir do id.
     *
     * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
     * @filesource
     * @throws Exception Mensagem de erro
     * @param int $param Identificador da assunto
     * @return object Dados de assunto
     */
    public function recuperaPorId($param)
    {
        try {
            $this->_modObra_estudo_demanda->carregarPorId($param);
            return (object) $this->_modObra_estudo_demanda->getDados();
        } catch (Exception $e) {
            simec_redirecionar($this->_url, 'error');
        }
    }

    /**
     * Método responsável por recuperar dados para a listagem
     *
     * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
     * @filesource
     * @throws Exception Mensagem de erro
     * @return object Dados de assunto
     */
    public function lista($dados = null)
    {
        try {
            $where = [];

            if ($dados['oeddsc']) {
                $where[] = "oeddsc ILIKE '%{$dados['oeddsc']}%'";
            }

            if ($dados['oedstatus'] && $dados['acao'] == 'pesquisar') {
                $where[] = "oedstatus = '{$dados['oedstatus']}'";
            }

            $whe = ($where ? 'WHERE ' : '').implode(' and ', $where);

            $sql = "SELECT oedid, oeddsc, oedstatus FROM par3.obra_estudo_demanda {$whe}";

            (new Simec_Listagem())
                ->setQuery($sql)
                ->setCabecalho(['Nome', 'Status'])
                ->setCampos(['oeddsc', 'oedstatus'])
                ->addAcao('edit', ['func' => 'editar', 'extra-params' => ['oeddsc', 'oedstatus']])
                ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        } catch (Exception $e) {
            simec_redirecionar($this->_url, 'error');
        }
    }
}

