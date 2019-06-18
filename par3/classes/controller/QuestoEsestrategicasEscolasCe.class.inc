<?php

/**
 * Classe de controle da escolas com ce
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_QuestoEsestrategicasEscolasCe
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */
class Par3_Controller_QuestoEsestrategicasEscolasCe extends Controle
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
     * - monta o formulario de escolas com CE
     *
     * @return string escreve a lista.
     *
     */
    public function formEscolasCe()
    {
        require_once APPRAIZ.'par3/modulos/principal/planoTrabalho/questoesEstrategicas/escolas_ce.php';
    }//end formEscolasCe()

    public static function formatarEntidComoCheckbox($co_entidade, $co_entidade_salvo, $arrPost) {
        $checkbox =  '<input type=checkbox class="js-switch-txt child-ck ';
        $checkbox .= $arrPost['class'].'" ';
        $checkbox .= ($co_entidade_salvo != '') ? 'checked="checked"' : '';
        $checkbox .= " qrpid={$arrPost['qrpid']} ";
        $checkbox .= " perid={$arrPost['perid']} ";
        $checkbox .= " co_entidade=$co_entidade />";

        return $checkbox;
    }


    /**
     * Função listaEscolasCe
     * - monta a lista de escolas com ce.
     *
     * @return escreve a lista.
     *
     */
    public function listaEscolasCe($arrPost, $onlySQL = false)
    {
        $sql = Par3_Model_QuestoesEstrategicasEscolasCe::pegarSQLLista($arrPost);
        if ($onlySQL) {
            return $sql;
        }

        $cabecalho = array('', 'Município', 'Código da Escola', 'Escola');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColuna('id');
        $listagem->addCallbackDeCampo('co_entidade', function($co_entidade, $dados, $id) use ($arrPost){
            return self::formatarEntidComoCheckbox($id, $co_entidade, $arrPost);
        });
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
    public function salvarEscolaCE($arrPost)
    {
        $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasCe();

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
        return $modelEscola->commit();
    }//end salvarEscolaCE()

    /**
     * @param $arrPost
     * @return bool
     */
    public function deleteEscolaCE($arrPost)
    {
        $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasCe();
        $modelEscola->excluirPorQuestionarioPeriodo($arrPost);
        return $modelEscola->commit();
    }

    /**
     * @param $arrPost
     * @return array
     */
    public function listaSelecionadas($arrPost)
    {
        $arrResponse=[];
        $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasCe();
        if ($rs = $modelEscola->retornaSelecao($arrPost)) {
            foreach ($rs as $row) {
                array_push($arrResponse, $row['co_entidade']);
            }
        }
        return $arrResponse;
    }
}//end class

?>