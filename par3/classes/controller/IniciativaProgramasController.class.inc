<?php

class Par3_Controller_IniciativaProgramasController extends Controle
{
    private $model;
    private $modelHistorico;
    public function __construct()
    {
        $prgid = validarInteiro($_REQUEST['prgid'])? $_REQUEST['prgid']:'';
        $this->model = new Par3_Model_IniciativaProgramas($prgid);
        $this->modelHistorico = new Par3_Model_IniciativaProgramasHistorico();
    }

    public function form($arrPost)
    {
        global $simec;

        $sql = "SELECT
                    pgoid as codigo,
                    pgodsc as descricao
                FROM par3.programaorigem
               ";
//ver($this->model,d);
        echo '<input type="hidden" name="prgid" value="'.$this->model->prgid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->prgid.'" class="form-control"></div>';
        echo '</div>';
        //echo $simec->select("pgoid", 'Tipo programa', $this->model->pgoid, $sql, array());
        echo $simec->input('prgabreviatura', 'Sigla', $this->model->prgabreviatura, array('maxlength' => '100'));
        echo $simec->input('prgdsc', 'Programa', $this->model->prgdsc, array('maxlength' => '255','required'));
        echo $simec->input('prglink', 'Link', $this->model->prglink, array('maxlength' => '255'));
        echo $simec->textarea('prgresumo', 'Resumo', $this->model->prgresumo);
        echo $simec->radio('prgatd_extraordinario', 'Atendimento Extraordinário',(!($this->model->prgatd_extraordinario)?'f':$this->model->prgatd_extraordinario), array('t' => 'Sim', 'f' => 'Não'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
        echo $simec->radio('prgsituacao', 'Situação',(!($this->model->prgsituacao)?'A':$this->model->prgsituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    public function salvar($arrPost)
    {
        //ver($arrPost,d);
        $acao = Par3_Model_IniciativaProgramasHistorico::CREATE;
        if( !empty($arrPost['prgid']) ) {
            $arrayProgramas['prgid']          = $arrPost['prgid'];
            $acao   = Par3_Model_IniciativaProgramasHistorico::UPDATE;
        }else {
            $arrayProgramas['prgdtinclusao']  = date('Y-m-d H:m:s');
        }

        $arrayProgramas['prgatd_extraordinario_old'] = '';
        if($arrPost['prgid']){
            $modelPrograma = new Par3_Model_IniciativaProgramas($arrPost['prgid']);//recupera programa
            $arrayProgramas['prgatd_extraordinario_old'] = $modelPrograma->prgatd_extraordinario;
        }

        $arrayProgramas['prgsituacao']           = $arrPost['prgsituacao'];
        $arrayProgramas['prgstatus']             = 'A';
        $arrayProgramas['pgoid']                 =  $arrPost['pgoid'] ? $arryPost['pgoid'] : null;
        $arrayProgramas['prgabreviatura']        = trim(($arrPost['prgabreviatura']));
        $arrayProgramas['prgdsc']                = trim(($arrPost['prgdsc']));
        $arrayProgramas['prglink']               = trim(($arrPost['prglink']));
        $arrayProgramas['prgresumo']             = trim(($arrPost['prgresumo']));
        $arrayProgramas['prgatd_extraordinario'] = $arrPost['prgatd_extraordinario'];
        $arrayProgramas['prgcpfinclusao']        = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);


        $arrayProgramas['prgperiodoinicio']      = null;
        $arrayProgramas['prgperiodofim']         = null;
        $arrayProgramas['prganoreferencia']      = null;
        $arrayProgramas['prgarquivo']            = null;
        $arrayProgramas['arqid']                 = null;
        $arrayProgramas['prgesfera']             = null;

        if(!empty($arrPost['prgid']) && $arrayProgramas['prgatd_extraordinario'] == 't'){
            $arrayProgramas['prgperiodoinicio']      = formata_data_sql($arrPost['prgperiodoinicio']);
            $arrayProgramas['prgperiodofim']         = formata_data_sql($arrPost['prgperiodofim']);
            $arrayProgramas['prganoreferencia']      = $arrPost['prganoreferencia'];
            $arrayProgramas['arqid']                 = trim($arrPost['arqid']);
            $arrayProgramas['prgesfera']             = $arrPost['prgesfera'];
        }

        //Valida campos
        $erro = $this->model->validarInput($arrayProgramas);
        if($erro) return $erro;

        try {
            $this->model->popularDadosObjeto($arrayProgramas);
            $id  = $this->model->salvar();
            $this->model->commit();

            $modelPrograma = new Par3_Model_IniciativaProgramas($id);//recupera programa
            $this->modelHistorico->gravarHistorico($modelPrograma,$acao);//grava em hitórico
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($id) {
            return $id;
        }

        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayProgramas['prgid']            = $arrPost['prgid'];
        $arrayProgramas['prgstatus']        = 'I';
        //$arrayProgramas['prgsituacao']      = 'I';
        $arrayProgramas['prgdtinativacao']  = date('Y-m-d H:m:s');
        $arrayProgramas['prgcpfinativacao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        try {
        	// Verifica se existe iniciativa (par3.iniciativa) relacionada. Se tiver, não é possível remover/inativar e então retorna erro.
        	$modelIniciativa = new Par3_Model_Iniciativa();
        	$sqlIniciativa = $modelIniciativa->montarSQLSimples(array('prgid'=>$arrayProgramas['prgid']), null);
        	$iniciativa = $modelIniciativa->carregar($sqlIniciativa);

        	if($iniciativa){
        		if(count($iniciativa) > 1){
        			return array('erros'=>array('existemIniciativasRelacionadas'=>$iniciativa));
        		} else {
        			return array('erros'=>array('existeIniciativaRelacionada'=>$iniciativa));
        		}
        	} else {
        		// Verifica se existe área relacionada (par3.iniciativa_areasrelacionadas_programas). Se tiver, não é possível remover/inativar e então retorna erro.
        		$modelIniAreRelacionadaPrg = new Par3_Model_IniciativaAreasRelacionadasProgramas();
        		$areRelacionadaPrg = $modelIniAreRelacionadaPrg->recuperarTodos('*', array("prgid = " .$arrayProgramas['prgid']   ." AND iapstatus = 'A'"), null);
        		if($areRelacionadaPrg){
        			if(count($areRelacionadaPrg) > 1){
        				return array('erros'=>array('existemAreasRelacionadas'=>$areRelacionadaPrg));
        			} else {
        				$modelIniciativaAreas = new Par3_Model_IniciativaAreas();
        				$iniciativaAreas = $modelIniciativaAreas->recuperarTodos('*', array("iarid = " .$areRelacionadaPrg[0]['iarid']), null);
        				return array('erros'=>array('existeAreaRelacionada'=>$iniciativaAreas));
        			}
        		// Não existe área relacionada (par3.iniciativa_areasrelacionadas_programas) nem iniciativa (par3.iniciativa). Continua a remoção/inativação.
	        	} else {
		            $this->model->popularDadosObjeto($arrayProgramas);
		            $sucesso = $this->model->salvar();
		            $this->model->commit();

		            $acao = Par3_Model_IniciativaProgramasHistorico::DELETE;
		            $modelPrograma = new Par3_Model_IniciativaProgramas($arrayProgramas['prgid'] );
		            $this->modelHistorico->gravarHistorico($modelPrograma,$acao);
	        	}
        	}
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function recuperar()
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        global $disabled;

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'prgid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'prgdsc';
                break;
            case 'prgabreviatura':
                $arrQuery['ordenacao']['campo'] = 'prgabreviatura';
                break;
            case 'prgsituacao':
                $arrQuery['ordenacao']['campo'] = 'prgsituacao';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }

        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Sigla', 'Programa', 'Atendimento Extraordinário', 'Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
        }
        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setId('listTipoObjeto');
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('prgsituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverPrograma');
            $listagem->addAcao('edit', 'editar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

//    private function salvarHistorico($id, $status){
//        $modelIniDesc = new Par3_Model_IniciativaTiposObjeto($id);
//
//        $arrIniDesHistorico['inprghdt']           = date('Y-m-d H:m:s');
//        $arrIniDesHistorico['inprghcpf']          = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
//        $arrIniDesHistorico['inprgid']            = $modelIniDesc->inprgid;
//        $arrIniDesHistorico['inprgdsc']           = $modelIniDesc->inprgdsc;
//        $arrIniDesHistorico['inprgstatus']        = $modelIniDesc->inprgstatus;
//        $arrIniDesHistorico['inprgsituacao']      = $modelIniDesc->inprgsituacao;
//        $arrIniDesHistorico['inprgdtinclusao']    = $modelIniDesc->inprgdtinclusao;
//        $arrIniDesHistorico['inprgcpfinclusao']   = $modelIniDesc->inprgcpfinclusao;
//        $arrIniDesHistorico['inprgdtinativacao']  = $modelIniDesc->inprgdtinativacao;
//        $arrIniDesHistorico['inprgcpfinativacao'] = $modelIniDesc->inprgcpfinativacao;
//        $arrIniDesHistorico = array_merge($arrIniDesHistorico, $status);
//
//        $modelIniDescHistorico = new Par3_Model_IniciativaTiposObjetoHistorico();
//        $modelIniDescHistorico->popularDadosObjeto($arrIniDesHistorico);
//        $modelIniDescHistorico->salvar();
//        $modelIniDescHistorico->commit();
//    }

    public function getIniciativaProgramaById($prgid){
        $result = $this->model->getIniciativaProgramaById($prgid);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($prgid){
        $result = $this->model->verificaLigacaoPlanejamentoById($prgid);
        return $result;
    }
}