<?php
/**
 * Classe de controle do CursosAbas
 * @category Class
 * @package  A
 * @version $Id$
 * @author   JANAINA ARQUE <janainarque@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25-04-2017
 * @link     no link
 */

/**
 * Par3_Controller_CursosAbas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25-04-2017
 * @link     no link
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";


class Par3_Controller_CursosAbas extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_CursosAbas($_REQUEST['curid']);
        $this->modelHistorico = new Par3_Model_CursosAbasHistorico();
    }

    public function form($arrPost)
    {
        global $simec;
        echo '<input type="hidden" name="curid" value="'.$this->model->curid.'"/>';
        echo $simec->input('curdsc', 'Descrição', $this->model->curdsc, array('maxlength' => '255','required','title' => 'Insira a descrição do Curso/Aba'));
        echo $simec->input('curtitulo', 'Título', $this->model->curtitulo, array('maxlength' => '255','title' => 'Insira o Título do Curso/Aba'));
        echo $simec->input('curqtd', 'Quandidade de Registros', $this->model->curqtd, array('title' => 'Insira a Quantidade de Registros do Curso/Aba', 'type' => 'number'));

        $sql = "SELECT funid as codigo, fundsc as descricao FROM entidade.funcao";
        echo $simec->select('funid', 'Função entidade', $this->model->funid, $sql, array('maxlength' => '255', null));

        echo $simec->textarea('curorientacao', 'Orientações',  $this->model->curorientacao, array('class' => 'summernote'));
        echo $simec->textarea('curmsg', 'Mensagem de sucesso', $this->model->curmsg, array('label-size' => '4', 'input-size' => '8'));


        $arrfuncaoatual = "select pffid as codigo, pffdescricao as descricao
								from par.pffuncao
								where pffstatus = 'A'
								order by pffdescricao";

        echo '<section class="form-group"><label class="col-lg-3 control-label">Função Atual:</label><section class="col-sm-9 col-md-9 col-lg-9 ">';
        echo inputCombo("pffid[]",$arrfuncaoatual, $this->recuperarCursoAbaPff( $this->model->curid ), 'pffid[]', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
        echo '</section></section>';


        $arrformacao = "select tfoid as codigo, tfodsc as descricao
                        from public.tipoformacao
                        where tfostatus = 'A'
                        order by tfodsc";
        echo '<section class="form-group"><label class="col-lg-3 control-label">Formação:</label><section class="col-sm-9 col-md-9 col-lg-9 ">';
        echo inputCombo("tfoid[]", $arrformacao, $this->recuperarCursoAbaTfo( $this->model->curid ),'tfoid[]', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
        echo '</section></section>';

        echo $simec->textarea('curscript', 'Scripts', $this->model->curscript, array('label-size' => '4', 'input-size' => '8'));
        echo $simec->radio('cursituacao', 'Situação',(!($this->model->cursituacao)?'A':$this->model->cursituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */

    public function recuperarCursoAbaPff($curid)
    {
        if($curid) {
            $selectPff = "select pffid as codigo from par3.curso_aba_pff where curid = {$curid}";
            $rsPff = $this->model->carregar($selectPff);
        }

        $arrPff = array();
        if(is_array($rsPff)){
            foreach ($rsPff as $c){
                $arrPff[] = $c['codigo'];
            }
        }
        return $arrPff;

    }

    public function recuperarCursoAbaTfo($curid)
    {
        if($curid) {
            $selectTfo = "select tfoid as codigo from par3.curso_aba_tfo where curid = {$curid}";
            $rsTfo = $this->model->carregar($selectTfo);
        }

        $arrTfo = array();
        if(is_array($rsTfo)){
            foreach ($rsTfo as $c){
                $arrTfo[] = $c['codigo'];
            }
        }
        return $arrTfo;
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_CursosAbasHistorico::CREATE;
        if($arrPost['curid']){
            $arrayCursosAbas['curid'] = $arrPost['curid'];
            $acao = Par3_Model_CursosAbasHistorico::UPDATE;
        }

        $arrayCursosAbas['cursituacao']         = $arrPost['cursituacao'];
        $arrayCursosAbas['inprgid']             = $arrPost['prgid'];
        $arrayCursosAbas['curstatus']           = 'A';
        $arrayCursosAbas['curdsc']              = trim(($arrPost['curdsc']));
        $arrayCursosAbas['curtitulo']           = trim(($arrPost['curtitulo']));

        if(!empty ($arrPost['curqtd'])){
            $arrayCursosAbas['curqtd']          = ($arrPost['curqtd']);
        }

        if(!empty($arrPost['funid'])){
            $arrayCursosAbas['funid']           = ($arrPost['funid']);
        }

        $arrayCursosAbas['curorientacao']       = trim(($arrPost['curorientacao']));
        $arrayCursosAbas['curmsg']              = ($arrPost['curmsg']);
        $arrayCursosAbas['curscript']           = trim(($arrPost['curscript']));
        $arrayCursosAbas['curcpfinclusao']      = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);

        if(!$arrPost['curid']){$arrayCursosAbas['curdtinclusao']  = date('Y-m-d H:m:s');}

        //Valida campos
        $erro = $this->model->validarInput($arrayCursosAbas);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayCursosAbas);
            $id = $this->model->salvar();
            $this->model->commit();


            if(!empty($arrPost['curid'])){
                $query_delete = "DELETE FROM par3.curso_aba_pff where curid = {$arrPost['curid']}";
                $this->model->executar($query_delete);
            }
            if (!empty($arrPost['pffid']) || $arrPost['pffid'] != null) {
                $dtInclusao = date('Y-m-d H:m:s');
                $usucpf = $_SESSION['usucpf'];

                foreach ($arrPost['pffid'] as $key) {
                    $sql = "INSERT INTO par3.curso_aba_pff(
                                curid, pffid, curdtinclusao, curcpfinclusao)
                               VALUES ($id, {$key}, '$dtInclusao', '$usucpf')";
                    $this->model->executar($sql);
                    $this->model->commit();
                }
            }

            if(!empty($arrPost['curid'])){
                $query_delete = "DELETE FROM par3.curso_aba_tfo where curid = {$arrPost['curid']}";
                $this->model->executar($query_delete);
            }

            if (!empty($arrPost['tfoid']) || $arrPost['tfoid'] != null) {
                $dtInclusao = date('Y-m-d H:m:s');
                $usucpf = $_SESSION['usucpf'];

                foreach ($arrPost['tfoid'] as $key) {
                    $sql1 = "INSERT INTO par3.curso_aba_tfo(
                                curid, tfoid, curdtinclusao, curcpfinclusao)
                               VALUES ($id,{$key},'$dtInclusao', '$usucpf')";
                    $this->model->executar($sql1);
                    $this->model->commit();
                }
            }

            $modelCursosAbas= new Par3_Model_CursosAbas($id);

            $this->modelHistorico->gravarHistorico($modelCursosAbas,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function desvincular($arrPost)
    {
        $arrayCursosAbas['curid']           = $arrPost['curid'];
        $arrayCursosAbas['cursituacao']     = 'I';
        $arrayCursosAbas['curdtinativacao'] = date('Y-m-d H:m:s');

        try {
            $this->model->popularDadosObjeto($arrayCursosAbas);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelCursosAbas = new Par3_Controller_CursosAbas($sucesso);

            $this->modelHistorico->gravarHistorico($modelCursosAbas,Par3_Model_CursosAbas_historico::DELETE);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($sucesso) {return $sucesso;}
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayCursosAbas['curid']             = $arrPost['curid'];
        $arrayCursosAbas['curstatus']         = 'I';
        $arrayCursosAbas['curdtinativacao']   = date('Y-m-d H:m:s');
        $arrayCursosAbas['curcpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayCursosAbas);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelCursosAbas = new Par3_Model_CursosAbas($arrPost['curid']);

            $this->modelHistorico->gravarHistorico($modelCursosAbas,Par3_Model_CursosAbasHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        global $disabled;
        $modelCursosAbas = new Par3_Model_CursosAbas();

        $sql = $modelCursosAbas->montarSQLSimples($arrPost);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: função, descrição, orientação, situação
        $cabecalho = array('Função','Descrição', 'Possui Orientações', 'Situação');

        $esconderColunas = array('id');

        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;


        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('CursosAbas');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->addCallbackDeCampo('cursituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerCursosAbas');
            $listagem->addAcao('edit', 'editarCursosAbas');
        }
        $listagem->setTamanhoPagina(10);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);

    }
}