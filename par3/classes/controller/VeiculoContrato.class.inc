<?php
/**
 * Classe de controle do  veiculo_contrato
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */


/**
 * Par3_Controller_Veiculo_contrato
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */
class Par3_Controller_VeiculoContrato
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_VeiculoContrato($_POST['vcoid']);
        $this->modelHistorico = new Par3_Model_VeiculoContratoHistorico();
    }

    public function formVeiculo($arrPost)
    {
        $veiculo = array('vcoid' => '', 'conid' => '', 'vcoplaca' => '', 'vcochassi' => '', 'vcorenavam' => '', 'mveid' => '', 'vcoano' => '', 'vcoqtdassentos' => '');

        if ($arrPost && !$this->model->vcoid) {
            $this->model->conid          = $arrPost['conid'];
            $this->model->vcochassi      = $arrPost['vcochassi'];
            $this->model->vcorenavam     = $arrPost['vcorenavam'];
            $this->model->vcoano         = $arrPost['vcoano'];
            $this->model->vcoplaca       = $arrPost['vcoplaca'];
            $this->model->mveid          = $arrPost['mveid'];
            $this->model->vcoqtdassentos = $arrPost['vcoqtdassentos'];
            $this->model->vcochassi      = $arrPost['vcoinscricao'];
            $this->model->arqid          = $arrPost['arqid'];
        }

        $modelModeloVeiculo = new Par3_Model_ModeloVeiculo($this->model->mveid);

        $arrPost['fabid'] = $arrPost['fabrica'];
        $rsModelos = $modelModeloVeiculo->pegarSQLSelectCombo($arrPost['fabid']);

        $fabricante = new Par3_Model_FabricanteVeiculo();
        $rsFabricantes = $fabricante->pegarSQLSelectListagemCombo($arrPost);
        $contrato = new Par3_Model_Contratos();
        $sqlContratos = $contrato->pegarSQLSelectCombo($arrPost);

        $arrPost['vcopossuirenavam'] = "t";
        $displayAutomovel = '';
        $displayEmarcacao = 'display:none';
        if ($this->model->vcopossuirenavam == 'f') {
            $displayAutomovel = 'display:none';
            $displayEmarcacao = '';
            $arrPost['vcopossuirenavam'] = $this->model->vcopossuirenavam;
        }

        $disabledTipo = $this->model->vcoid ? 'disabled' : '';

        global $simec;
        echo '<input type="hidden" name="vcoid" value="' . $this->model->vcoid . '" />';
        echo '<input type="hidden" name="vcopossuirenavam" id="vcopossuirenavam" value="' . $this->model->vcopossuirenavam . '" />';
        echo $simec->radio('vcopossuirenavam', 'Tipo de Veículo', $arrPost['vcopossuirenavam'], ['t' => 'Veículo', 'f' => 'Embarcação'], array($disabledTipo));

        echo '<div class="row"><div class="col-md-6" id="div-conid">' . $simec->select('conid', 'Nº Contrato', $this->model->conid, $sqlContratos, Array('required')) . '</div>';

        echo '<div class="row"><div class="col-md-6" style="' . $displayEmarcacao . '" id="div-vcoinscricao">' . $simec->input('vcoinscricao', 'Inscrição da Embarcação', $this->model->vcochassi,$arrPost['vcopossuirenavam'] == 'f' ? array('maxlength'=>'16','required') : array('maxlength'=>'16')) . '</div>';

        echo '<div class="col-md-6" style="' . $displayAutomovel . '" id="div-vcoplaca">' . $simec->input('vcoplaca', 'Placa do Veículo', $this->model->vcoplaca, $arrPost['vcopossuirenavam'] != 'f' ? array('maxlength'=>'11','required') : array('maxlength'=>'11')) . '</div></div>';
        echo '<div class="row"><div class="col-md-6" style="' . $displayAutomovel . '" id="div-vcochassi">' . $simec->input('vcochassi', 'Chassi', $this->model->vcochassi,  $arrPost['vcopossuirenavam'] != 'f' ? array('maxlength'=>'17','required') : array('maxlength'=>'17')) . '</div>';
        echo '<div class="col-md-6 input-group" style="' . $displayAutomovel . '" id="div-fabricante">' . $simec->select('fabricante', 'Fabricante', $modelModeloVeiculo->fabid, $rsFabricantes,$arrPost['vcopossuirenavam'] != 'f' ? array('required') : array('')) . '
                <span class="input-group-btn">
                    <button type="button" class="btn btn-success" style="margin-top:-10px;" data-toggle="modal"data-target="#modal_fabricante" title="Novo Fabricante">
                     <i class="fa fa-plus"></i>
                    </button>
                </span>
              </div></div>';
        echo '<div class="row"><div class="col-md-6" style="' . $displayAutomovel . '" id="div-vcorenavam">' . $simec->input('vcorenavam', 'RENAVAM', $this->model->vcorenavam, array('maxlength'=>'11','required'), array('')) . '</div>';
        echo '<div class="col-md-6 input-group" style="' . $displayAutomovel . '" id="div-mveid">' . $simec->select('mveid', 'Modelo', $this->model->mveid, $rsModelos, $arrPost['vcopossuirenavam'] != 'f' ? array('required') : array('')) . '
                <span class="input-group-btn">
                    <button type="button" class="btn btn-success" style="margin-top:-10px;" data-toggle="modal"data-target="#modal_modelo" title="Novo modelo">
                     <i class="fa fa-plus"></i>
                    </button>
                </span>
              </div></div>';

        echo '<div class="row"><div class="col-md-6" id="div-vcoano">' . $simec->input('vcoano', 'Ano do Veículo', $this->model->vcoano,array('type'=>'number','min'=>'1','max'=>'3000','required')) . '</div>';
        echo '<div class="col-md-6">' . $simec->input('vcoqtdassentos', 'Quantidade de Assentos', $this->model->vcoqtdassentos, $arrPost['vcopossuirenavam'] != 'f' ? array('type'=>'number','min'=>'1','max'=>'999','required') : array('type'=>'number','min'=>'1','max'=>'999')) . '</div></div>';
        echo '<div class="row"><div class="col-md-6" style="' . $displayAutomovel . '"><div id="div-arqid">';
        if (!empty($this->model->arqid)) {
            $link.= "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid={$_REQUEST['inuid']}&req=baixarAutorizacao&vcoid={$this->model->vcoid}' target='blank'><small style='font-size: 12px;'><i class='fa fa-download'></i> Abrir Anexo</small></a> | ";
            $link.= "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid={$_REQUEST['inuid']}&req=excluirAutorizacao&vcoid={$this->model->vcoid}'><small style='font-size: 12px;'> <i class='fa fa-trash'></i> Excluir</small></a>";
            echo '<div class="form-group">
                <label for="arqid" class="col-sm-3 col-md-3 col-lg-3 control-label">Autorização do veículo para condução dos escolares: *</label>
                <div class="col-sm-9 col-md-9 col-lg-9"><div class="input-group" style="margin-top: 4px">'.$link.'</div></div>
            </div>';
        } else {
            echo $simec->input('arqid', 'Autorização do veículo para condução dos escolares', null, array('type' => 'file', 'required', 'help' => 'Atr. 136 do Código de Transito Brasileiro'));
        }
        echo '</div></div>';
        echo '<div class="col-md-6">
                <input type="hidden" name="req" value="salvarVeiculo"/>
                <div class="pull-right">
                <button type="button" class="btn btn-warning" id="atualizarFormVeiculo" title="Limpar Formulário">
                    <i class="fa fa-eraser"></i>
                </button>
                <button type="submit" class="btn btn-success" id="novo_veiculo" title="Salvar Veículo">
                    <i class="fa fa-save"></i>
                    Salvar Veículo
                </button>
              </div>
            </div></div>
            ';
    }

    public function listarVeiculos($arrPost)
    {
        global $disabled;
        $mVeiculoContrato = new Par3_Model_VeiculoContrato();
        $sql = $mVeiculoContrato->montarSQLSimplesVeiculos($arrPost);
        $cabecalho = array('N° Contrato', 'Placa', 'Chassi', 'RENAVAM', 'Marca', 'Modelo', 'Ano', 'Assentos');

        if ($arrPost['vcopossuirenavam'] == 'f') {
            $cabecalho = array('N° Contrato', 'Inscrição da Embarcação', 'Ano', 'Assentos');

        }

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id','arqid']);
        if ($arrPost['vcopossuirenavam'] == 'f') {
            $listagem->esconderColunas(['id', 'vcoplaca', 'vcorenavam', 'mvedsc', 'fabdsc']);

        }
        if ($disabled == '') {
            $listagem->addAcao('remove', 'inativarVeiculo');
            $listagem->addAcao('edit', 'editarVeiculo');
            $listagem->addAcao('download', 'baixarAutorizacao');
        }
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }//end listaEquipe()

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;
        //define acão para guardar em histórico
        $acao = Par3_Model_VeiculoContratoHistorico::CREATE;
        if($arrPost['vcoid']){
            $arrayCiclo['vcoid'] = trim($arrPost['vcoid']);
            $acao = Par3_Model_VeiculoContratoHistorico::UPDATE;
        }

        $arrayVeiculo['vcodescricao']     = ($arrPost['vcodescricao']);
        if ($arrPost['vcopossuirenavam'] == 'f') {
            $arrayVeiculo['vcochassi']    = str_replace(array(' '),"",$arrPost['vcoinscricao']);
        }

        if($arrPost['vcopossuirenavam'] == 't'){
            $arrayVeiculo['vcochassi']    = trim($arrPost['vcochassi']);
            $arrayVeiculo['mveid']        = trim($arrPost['mveid']);
            $arrayVeiculo['vcoplaca']     = trim($arrPost['vcoplaca']);
            $arrayVeiculo['vcorenavam']   = str_replace(array(' '),"",$arrPost['vcorenavam']);
        }

        $arrayVeiculo['vcoid']            = trim($arrPost['vcoid']);
        $arrayVeiculo['arqid']            = trim($arrPost['arqid']);
        $arrayVeiculo['vcoqtdassentos']   = trim($arrPost['vcoqtdassentos']);
        $arrayVeiculo['vcoano']           = trim($arrPost['vcoano']);
        $arrayVeiculo['vcopossuirenavam'] = trim($arrPost['vcopossuirenavam']);
        $arrayVeiculo['conid']            = trim($arrPost['conid']);
        $arrayVeiculo['usucpf']           = str_replace(array(' ','.','-'),"",$_SESSION['usucpf']);
        $arrayVeiculo['vcostatus']        = 'A';
        $arrayVeiculo['vcodtcricao']      = date('Y-m-d H:m:s');
        $arrayVeiculo['vcochassi']        = str_replace(array('.', '-', '/', ' '), '', $arrayVeiculo['vcochassi']);
        try {
            /*@TODO
             * validar renavam
             */
//            if ($arrPost['vcopossuirenavam'] != 'f') {
//                if(!validaRenavam($arrPost['vcorenavam'])){return 'renavam';}
//                if (strlen($arrPost['vcochassi']) > 16) {return 'chassi';}
//            }

            if (!(strlen($arrPost['vcoano']) == 4 && is_numeric($arrPost['vcoano']))) {return 'ano';}
            if (strlen($arrPost['vcorenavam']) > 11) {return 'renavam';}

            $this->model->popularDadosObjeto($arrayVeiculo);
            $novoVeiculoContrato = $this->model->salvar();
            $this->model->commit();

            $modelVeiculoContrato = new Par3_Model_VeiculoContrato($novoVeiculoContrato);
            $mHistorico = new Par3_Model_VeiculoContratoHistorico();
           // $mHistorico->gravarHistorico($modelVeiculoContrato,$acao);

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($novoVeiculoContrato) {

            $novoVeiculoContrato = (int)$novoVeiculoContrato;
            return $novoVeiculoContrato;
        }
        return 'erro';
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['vcoid'];
        $url = "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid={$arrPost['vcoid']}";
        $veiculo_contrato = new Par3_Model_VeiculoContrato($id);
        try {
            $veiculo_contrato->vcostatus = 'I';

            $veiculo_contrato->salvar();
            $veiculo_contrato->commit();

            $modelVeiculoContrato = new Par3_Model_VeiculoContrato($arrPost['vcoid']);

            $mHistorico = new Par3_Model_VeiculoContratoHistorico();
            $mHistorico->gravarHistorico($modelVeiculoContrato,Par3_Model_VeiculoContratoHistorico::DELETE);
            simec_redirecionar($url, 'success');
            return 'sucesso';
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }


}

?>