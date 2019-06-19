<?php

class Par3_Controller_ObraXQuestionario
{
    private $model;
    public $simec;

    public function __construct($obrid, $oabid = null)
    {
        $this->simec = new Simec_View_Helper();
        $this->modelObraXQuestionario = new Par3_Model_ObraXQuestionario($obqeid = null, $obrid, $oabid);
        if(isset($_REQUEST['obqeid']) && $_REQUEST['obqeid'] != ''){
            $this->modelObraXQuestionario->create($_REQUEST['obqeid']);
        }

        $this->modelObraXQuestionario->salvar();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;

        $inuid = (int)$_REQUEST['inuid'];
        $obrid = (int)$_POST['obrid'];
        if(isset($_REQUEST['toReturn'])){
            $url .= "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=relatorio_vistoria&inuid={$inuid}&inpid={$_REQUEST['inpid']}&obrid={$obrid}&toReturn={$_REQUEST['toReturn']}";
        }else{
            $url .= "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=relatorio_vistoria&inuid={$inuid}&inpid={$_REQUEST['inpid']}&obrid={$obrid}";
        }

        if(!$obrid) simec_redirecionar($url, 'error', 'ID de obra inválido');


//        $modelObraPar3 = new Par3_Model_Obra();
//        $dbObraPar3 = $modelObraPar3->create($_POST['obrid']);
//
//        if($dbObraPar3['octid'] == Par3_Model_ObraTipo::AMPLIACAO){
//
//        }

        $boo = '';
        if($_POST['obrid_reforma'] > 0){
            $boo = $this->modelObraXQuestionario->verificaObra($_POST['obrid_reforma'], $_POST['inuid']);
        }else{
            $_POST['obrid_reforma'] = null;
            $_REQUEST['obrid_reforma'] = null;
            $boo = true;
        }


        //$boo = true;
        if($boo){
            $this->modelObraXQuestionario->popularDadosObjeto($_POST);
            $this->modelObraXQuestionario->getRegraQuestionario();
            try {
                $id = $this->modelObraXQuestionario->salvar(true, true, array('obqeid', 'qrpid', 'obrid_reforma', 'escid', 'cod_escola'));
                if(isset($_REQUEST['toReturn'])){
                    $url .= "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=relatorio_vistoria&inuid={$inuid}&inpid={$_REQUEST['inpid']}&obrid={$obrid}&obqeid={$id}&toReturn={$_REQUEST['toReturn']}";
                }else{
                    $url .= "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=relatorio_vistoria&inuid={$inuid}&inpid={$_REQUEST['inpid']}&obrid={$obrid}&obqeid={$id}";
                }
                $this->modelObraXQuestionario->commit();
                return $id;
                //simec_redirecionar($url, 'success');
            } catch (Simec_Db_Exception $e) {
                simec_redirecionar($url, 'error');
            }
//            if ($id) {
//                simec_redirecionar($url, 'success');
//            }
//            simec_redirecionar($url, 'error');
//        }else{
//            simec_redirecionar($url, 'error', 'ID de obra inválido');
        }
    }

    public function getFormularioVistoria($inuid, $obrid)
    {
        //VALIDAR EDIÇÃO PARA ANÁLISE
        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $disabledAnalise = $mpergunta ->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::RELATORIO_VISTORIA,1,$_GET['obrid']);
        //*******************************************

        switch ($this->modelObraXQuestionario->modelObraTipo->octid) {

            case Par3_Model_ObraTipo::CONSTRUCAO:
                $this->queid = QUESTIONARIO_OBRA_VISTORIA;
                break;
            case Par3_Model_ObraTipo::AMPLIACAO:
                $display = '';
                $booVizualiza = true;

                if($this->modelObraXQuestionario->beneficiada_fnde != 'S'){
                    $arrayConfigAmpliacaoMesmoTerreno = array(
                        'required' => 'required'
                    );
                    $booVizualiza = false;
                }else{
                    if($this->modelObraXQuestionario->apliacao_mesmo_terreno != 'S'){ $display = "none"; }
                    $arrayConfigAmpliacaoMesmoTerreno = array();
                }

                //echo $this->simec->input('obqeid', '', $_REQUEST['obqeid'], array('type' => 'hidden'));

                echo "<div id=\"div_beneficiada_fnde\" class=\"form-group\">";
                echo $this->simec->radio('beneficiada_fnde', 'A escola beneficiada é uma escola padrão FNDE?',
                    trim($this->modelObraXQuestionario->beneficiada_fnde), array('S' => 'SIM', 'N' => 'NÃO'), array('required' => 'required',$disabledAnalise), array('label-size' => '5', 'input-size' => 5)
                );
                echo "<span id=\"span_beneficiada_fnde\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";

                echo "<div id=\"div_obrid_reforma\" class=\"form-group\">";
                echo $this->simec->input('obrid_reforma', 'ID Obra', $this->modelObraXQuestionario->obrid_reforma,
                    array('max' => '90000','type' => 'number','style' => 'width:150px;',$disabledAnalise), array('label-size' => '5', 'input-size' => 5, 'visible' => $booVizualiza));
                echo "<span id=\"span_obrid_reforma\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";

                echo "<div id=\"div_escid\" class=\"form-group\">";
                $escid = $this->modelObraXQuestionario->escid;
                echo $this->simec->select('escid', 'Selecionar escola',$escid, simec_preparar_array($this->modelObraXQuestionario->listarEscolas($obrid, $inuid,$escid)),
                    array('required',$disabledAnalise), array('label-size' => '5', 'input-size' => 5));
                echo "<span id=\"span_escid\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";

                echo "<div id=\"div_apliacao_mesmo_terreno\" class=\"form-group\">";
                echo $this->simec->radio('apliacao_mesmo_terreno', ' Ampliação é no mesmo terreno?', trim($this->modelObraXQuestionario->apliacao_mesmo_terreno), array('S' => 'SIM', 'N' => 'NÃO'),
                    array($disabledAnalise), array('label-size' => '5', 'input-size' => 5, 'visible' => $booVizualiza));
                echo "<span id=\"span_apliacao_mesmo_terreno\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";
                echo '<div id="div-escola-panel"></div>';

                if(!$disabledAnalise){
                echo ' <div class="col-md-12  text-center">
                            <button type="button" class="btn btn-success salvarQuestionarioVistoria"><i class="fa fa-save"></i> Salvar</button>
                        </div>';
                }

                echo "<div id=\"descricao_obra\" style=display:$display; ></div>";

                break;
            case Par3_Model_ObraTipo::REFORMA:
                $disable = array($disabledAnalise);
                if($this->modelObraXQuestionario->beneficiada_fnde){
                    //$disable = array('disabled'=>'disabled'); //#leo removidoParaNãoTravarMaisOFormularioDeVistoria
                }

                $booVizualiza = true;
                /*if(trim($this->modelObraXQuestionario->beneficiada_fnde) == 'N') //#leo removido, formulário não ficará travado
                    $booVizualiza = true;*/

                echo $this->simec->input('obqeid', '', $_REQUEST['obqeid'], array('type' => 'hidden'));

                echo "<div id=\"div_beneficiada_fnde\" class=\"form-group\">";
                echo $this->simec->radio('beneficiada_fnde', 'A escola beneficiada é uma escola padrão FNDE?*',
                    trim($this->modelObraXQuestionario->beneficiada_fnde), array('S' => 'SIM', 'N' => 'NÃO'), array_merge(array('required' => 'required'), $disable), array('label-size' => '5', 'input-size' => 5)
                );
                echo "<span id=\"span_beneficiada_fnde\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";


                echo "<div id=\"div_obrid_reforma\" class=\"form-group\">";
                echo $this->simec->input('obrid_reforma', 'ID Obra', $this->modelObraXQuestionario->obrid_reforma, array_merge(array('max' => '90000000','type' => 'number','style' => 'width:150px;'), $disable), array('label-size' => '5', 'input-size' => 5, 'visible' => $booVizualiza));
                echo "<span id=\"span_obrid_reforma\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";

                echo "<div id=\"div_cod_escola\" class=\"form-group\">";
                $cod_escola = $this->modelObraXQuestionario->cod_escola;
                echo $this->simec->select('cod_escola', 'Selecionar escola', $cod_escola, simec_preparar_array($this->modelObraXQuestionario->listarEscolas($obrid,$inuid,$cod_escola)),
                    array_merge(array(), $disable), array('label-size' => '5', 'input-size' => 5));
                echo "<span id=\"span_cod_escola\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                echo "</div>";

                echo '<div id="div-escola-panel"></div>';

                // ampliação
                if ($this->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::AMPLIACAO) {
                    echo $this->simec->radio('apliacao_mesmo_terreno', ' Ampliação é no mesmo terreno?', trim($this->modelObraXQuestionario->apliacao_mesmo_terreno), array('S' => 'SIM', 'N' => 'NÃO'),
                        array_merge(array('required' => 'required'), $disable), array('label-size' => '5', 'input-size' => 5));
                    echo "<span id=\"span_apliacao_mesmo_terreno\" class=\"span_error\" style='margin-left: 45%; display:none; color: red;'>Campo Obrigatório!</span>";
                }
                if(!$disabledAnalise) {
                    echo ' <div class="col-md-12  text-center">
                            <button type="submit" class="btn btn-success salvarQuestionarioVistoria"><i class="fa fa-save"></i> Salvar</button>
                        </div>';
                }

                echo "<div id=\"descricao_obra\"></div>";

                if(trim($this->modelObraXQuestionario->beneficiada_fnde) == 'S')
                    echo '<div id="descricao_obra"></div>';

                break;
        }
    }

    public function getQuestionario($obrid)
    {
        return $this->modelObraXQuestionario->getQuestionario($obrid);
    }

    public function retornarDocidQuestionario($obrid)
    {
        return $this->modelObraXQuestionario->retornarQrpidObra($obrid);
    }

    public function recuperarVigenciaQuestionario()
    {
        return $this->modelObraXQuestionario->recuperarVigenciaQuestionario();
    }

    public function carregarBotaoWorkflow($obrid)
    {
        return $this->modelObraXQuestionario->carregarBotaoWorkflow($obrid);
    }

    public function finalizarPreenchimentoQuestionario($obrid, $inuid)
    {
        return $this->modelObraXQuestionario->finalizarPreenchimentoQuestionario($obrid, $inuid);
    }

    public function reabrirPreenchimentoQuestionario($obrid, $inuid)
    {
        return $this->modelObraXQuestionario->reabrirPreenchimentoQuestionario($obrid, $inuid);
    }

    public function getDescricaoCategoriaTipoObra()
    {
        switch ($this->modelObraXQuestionario->modelObraTipo->octid) {
            case Par3_Model_ObraTipo::CONSTRUCAO:
                return 'Construção';
                break;
            case Par3_Model_ObraTipo::AMPLIACAO:
                return 'Ampliação';
                break;
            case Par3_Model_ObraTipo::REFORMA:
                return 'Reforma';
                break;
        }
    }


}

?>