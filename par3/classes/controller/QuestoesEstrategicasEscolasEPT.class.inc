<?php

/**
 * Classe de controle da escolas com EPT
 *
 * @category Class/Render
 * @package  A1
 * @author   Jair Foro <jairsantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_QuestoEsestrategicasEscolasEPT
 *
 * @category Class/Render
 * @package  A1
 * @author   Jair Foro <jairsantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */
class Par3_Controller_QuestoesEstrategicasEscolasEPT extends Controle
{
    /* Constantes da Equipe local
     * */
// 	const PREFEITURA          = 1;

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formEscolasCe
     * - monta o formulario de escolas com EPT
     *
     * @return string escreve a lista.
     *
     */
    public function formEscolasEPT()
    {
        require_once APPRAIZ.'par3/modulos/principal/planoTrabalho/questoesEstrategicas/escolas_ept.php';
    }//end formEscolasCe()

    public static function formatarEntidComoCheckbox($co_entidade, $co_entidade_salvo, $arrPost) {
        $checkbox =  '<input type=checkbox class="js-switch ';
        $checkbox .= $arrPost['class'].'" ';
        $checkbox .= ($co_entidade_salvo != '') ? 'checked="checked"' : '';
        $checkbox .= " qrpid={$arrPost['qrpid']} ";
        $checkbox .= " perid={$arrPost['perid']} ";
        $checkbox .= " co_entidade=$co_entidade />";

        return $checkbox;

    }


    /**
     * Função listaEscolasEPT
     * - monta a lista de escolas com EPT.
     *
     * @return escreve a lista.
     *
     */
    public function listaEscolasEPT($arrPost, $onlySQL = false)
    {
        $sql = Par3_Model_QuestoesEstrategicasEscolasEPT::pegarSQLLista($arrPost);
        if ($onlySQL) {
            return $sql;
        }

        $cabecalho = array('', 'Município', 'Código da Escola', 'Escola');

        function formatarco_entidadeComoCheckbox ($co_entidade, $dados, $id) {
            return Par3_Controller_QuestoesEstrategicasEscolasEPT::formatarEntidComoCheckbox($id, $co_entidade, $arrPost);
        }

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColuna('id');
        $listagem->addCallbackDeCampo('co_entidade', 'formatarco_entidadeComoCheckbox');
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render();
    }//end listaEscolasCe()


    /**
     * Função salvarEscolaCE
     * - inativa membro da equipe.
     *
     * @return void.
     *
     */
    public function salvarEscolaEPT($arrPost)
    {
        $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasEPT();

        $sql = "SELECT TRUE FROM questionario.resposta WHERE qrpid = {$arrPost['qrpid']} AND perid = {$arrPost['perid']}";

        $respondido = $modelEscola->pegaUm($sql);

        if($respondido != 't'){
            $sql = "INSERT INTO questionario.resposta(qrpid, perid, usucpf)
                    VALUES({$arrPost['qrpid']}, {$arrPost['perid']}, {$_SESSION['usucpf']});";
            $modelEscola->executar($sql);
            $modelEscola->commit();
        }

        $modelEscola->popularDadosObjeto($arrPost);
        $modelEscola->salvar();
        $modelEscola->commit();
    }//end salvarEscolaCE()

    /**
     * @param $arrPost
     * @return array
     */
    public function listaSelecionadas($arrPost)
    {
        $arrResponse=[];
        $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasEPT();
        if ($rs = $modelEscola->retornaSelecao($arrPost)) {
            foreach ($rs as $row) {
                array_push($arrResponse, $row['co_entidade']);
            }
        }
        return $arrResponse;
    }

    /**
     * @param $arrPost
     * @return bool
     */
    public function deleteEscolaEPT($arrPost)
    {
        $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasEPT();
        $modelEscola->excluirPorQuestionarioPeriodo($arrPost);
        return $modelEscola->commit();
    }
}//end class

?>