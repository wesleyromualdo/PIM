<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 7/11/2017
 * Time: 3:12 PM
 */

class Par3_Controller_GuiaPlanejamentoInformacaoDemograficaEducacional{

    private $modelIniciativa;
    private $modelIde;
    private $modelIniciativaIniciativasEstados;
    private $modeloIniciativaIndicadoresDemograficosStatus;
    private $modelIndicadorDemografico;

    public function __construct()
    {
        $this->modelIniciativa = new Par3_Model_Iniciativa();
        $this->ide = new Par3_Model_Ide();
        $this->modelIniciativaIniciativasEstados = new Par3_Model_IniciativaIniciativasEstados();
        $this->modeloIniciativaIndicadoresDemograficosStatus = new Par3_Model_IniciativaIndicadorDemograficoIndicadorStatus();
        $this->modelIndicadorDemografico = new Par3_Model_IndicadorDemografico();
    }


    public function gravarIniciativaIndicadorDemograficoStatus(array $arrayPopular){
        $this->modeloIniciativaIndicadoresDemograficosStatus->popularDadosObjeto($arrayPopular);
        $a = $this->modeloIniciativaIndicadoresDemograficosStatus->salvar();
        $this->modeloIniciativaIndicadoresDemograficosStatus->commit();
    }



    public function filtroTabelaIndicadores($arrRequest){
        $modelMunicipio = new Par3_Model_Municipio();

        if(!isset($arrRequest['iniid'])){
            $url =  "http://simec-local/par3/par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=listaIniciativas";
            $tipoRed = 'error';
            $msg = "É necessário incluir a Iniciativa.";
            simec_redirecionar($url,$tipoRed,$msg);
        }else {

            $modelIniciativa = new Par3_Model_Iniciativa();
            $getIniciativa = $modelIniciativa->getDadosIniciativaDadosDemograficos($_REQUEST);

            $this->ide->tipo = $modelIniciativa->retornaEsferaIniciativa($getIniciativa);

            $arrListaIndicadoresDemograficos = array();
            $descrMuniOrEstad = "";
            if ($this->ide->tipo == 'municipio') {
                $getMunicipio = $modelMunicipio->getMunicipio($getIniciativa['muncod']);
                $descMunicipio = $getMunicipio['mundescricao'];
                $descrMuniOrEstad = $descMunicipio;
                $this->ide->muncod = $getIniciativa['muncod'];
                $arrListaIndicadoresDemograficos = $this->arrayIndicadoresDemograficosExistentes($descMunicipio);
            } elseif ($this->ide->tipo == 'estado') {
                $this->ide->estuf = ($getIniciativa['estuf'] != '') ? $getIniciativa['estuf'] : null;
                $descrMuniOrEstad = $getIniciativa['estdescricao'];
                $arrListaIndicadoresDemograficos = $this->arrayIndicadoresDemograficosExistentes($getIniciativa['estdescricao']);
            }
        }

       include_once APPRAIZ . 'par3/modulos/principal/configuracao/listaGrupoIndicador.inc';
    }

    /**
     * @param $iniid Id da iniciativa
     * @param $tipoCheckBox 1 = plano de trabalho, 2 = analise técnica, 3 = analise merito
     * @param $indicadorTabela
     *
     */
    public function montaCheckBoxFiltro($iniid,$tipoCheckBox,$indicadorTabela){
        $check = $this->modeloIniciativaIndicadoresDemograficosStatus->validaSeCadastrado($iniid,$tipoCheckBox,trim(strtolower($indicadorTabela)));
        $checked = ($check == true) ? "checked='checked' " : '';
        $value = $iniid."|".$tipoCheckBox."|".$indicadorTabela;
        $html  = "<td style=\"text-align: center;\">";
        $html .= "<div class=\"icheckbox-square-green\">";
        if(isset($_REQUEST['req'])){
            switch ($_REQUEST['req']){
                case "xls":
                   if($check == true){$checked="SIM";}else{$checked="NÃO";};
                   $html .= $checked;
                    break;
                    case "imprimir":
                    if($check == true){$checked="SIM";}else{$checked="NÃO";};
                        $html .= $checked;
                    break;
                default:
                    $html .= "<input name=\"filterTableIndicadoresPlanoDeContas[]\" {$checked} id=\"plt\" value=\"$tipoCheckBox\" type=\"checkbox\" onclick=\"ajax_alterIndicadoresDemograficaos($iniid,$tipoCheckBox,'$indicadorTabela');\" class='checkbox checkbox-default checkboxPOST'>";
                    break;
            }
        }else{
            $html .= "<input name=\"filterTableIndicadoresPlanoDeContas[]\" {$checked} id=\"plt\" value=\"$tipoCheckBox\" type=\"checkbox\" onclick=\"ajax_alterIndicadoresDemograficaos($iniid,$tipoCheckBox,'$indicadorTabela');\" class='checkbox checkbox-default checkboxPOST'>";

        }
        $html .= "</div>";
        $html .= "</td>";
        return $html;

    }

    public function arrayIndicadoresDemograficosExistentes($descrEstOrMunic){
        $arrayIndicadores = $this->modelIndicadorDemografico->fetchAll();
        $arrIndicadoresDemograficos = array(
                          "03"  => "Índice de Desenvolvimento da Educação Básica - IDEB",
                          "06a" => "Número de Escolas por Etapa de Ensino - Rede Estadual em {$descrEstOrMunic}",
                          "06b" => "Número de Escolas por Etapa de Ensino - Rede Municipal em {$descrEstOrMunic}",
                          "07a" => "Número de Escolas Rurais em Áreas Específicas - Rede Estadual em {$descrEstOrMunic}",
                          "07b" => "Número de Escolas Rurais em Áreas Específicas - Rede Municipal em {$descrEstOrMunic}",
                          "08a" => "Número de Escolas por Modalidade e Etapa de Ensino - Rede Estadual em {$descrEstOrMunic}",
                          "08b" => "Número de Escolas por Modalidade e Etapa de Ensino - Rede Municipal em {$descrEstOrMunic}",
                          "09a" => "Matrículas por Modalidade, Etapa e Turno - Rede Estadual em {$descrEstOrMunic}",
                          "09b" => "Matrículas por Modalidade, Etapa e Turno - Rede Municipal em {$descrEstOrMunic}",
                          "09c" => "Distribuição de Matrículas do Ensino Médio por Modalidade - {$descrEstOrMunic}",
                          "10a" => "Funções Docentes por Modalidade e Etapa de Ensino - Rede Estadual em {$descrEstOrMunic}",
                          "10b" => "Funções Docentes por Modalidade e Etapa de Ensino - Rede Municipal em {$descrEstOrMunic}",
                          "11a" => "Condições de Atendimento do Diurno - Rede Estadual em {$descrEstOrMunic}",
                          "11b" => "Condições de Atendimento do Diurno - Rede Municipal em {$descrEstOrMunic}",
                          "12a" => "Condições de Atendimento - Noturno - Rede Estadual em {$descrEstOrMunic}",
                          "12B" => "Condições de Atendimento - Noturno - Rede Municipal em {$descrEstOrMunic}",
                          "13a" => "Taxas de Rendimento - Rede Estadual em {$descrEstOrMunic}",
                          "13b" => "Taxas de Rendimento - Rede Municipal em {$descrEstOrMunic}",
                          "14a" => "Matrículas em Turmas de Correção de Fluxo - Rede Estadual em {$descrEstOrMunic}",
                          "14b" => "Matrículas em Turmas de Correção de Fluxo - Rede Municipal em {$descrEstOrMunic}"
        );

        return $arrIndicadoresDemograficos;
    }
    public function arrayIndicadoresDemograficosExistentesKey(){
        $arrIndicadoresDemograficos = array("3","6A","6B","7A","7B","8A","8B","9A","9B","9C","10A","10B","11A","11B","12A","12B","13A","13B","14A","14B",);
        return $arrIndicadoresDemograficos;
    }

    function retornaDescricaoIndicadorHtmlTd($value){
        $html = "<td width='50%' style='text-align: left;'>".$value."</td>";
    }

    function montaTabela(Par3_Model_Ide $ide, $rede,$indicadorDemografico,$tabelaUF,$tabelaMU){
        $this->ide = $ide;
        $this->headTabelaTitulo($indicadorDemografico);
        $this->ide->getTitulo($rede, $tabelaUF, $tabelaMU);
        $this->footerTabelaTitulo();
        $this->ide->getTabela($rede, $tabelaUF, $tabelaMU);
        $this->footerTabelaTabela();

    }

    function headTabelaTitulo($indicadorDemografico){
        $indicadorDemografico = strtolower($indicadorDemografico);
        echo "<div id=\"$indicadorDemografico\"></div>";
            echo "<div class=\"row\" style=\"margin-top: 70px;\">";
                echo "<div class=\"col-lg-12\">";
                  echo "<div class=\"panel panel-primary\">";
                    echo "<div class=\"panel-heading\">";
    }

    function headTabela9SemIndicador(){
        echo "<div class=\"row\" style=\"margin-top: 70px;\">
    <div class=\"col-lg-12\">
        <div class=\"panel panel-success\">
            <div class=\"panel-heading\">";
    }

    function footerTabelaTitulo(){
        echo "</div>";
    }

    function headTabelaTabela(){
        echo "<div class=\"panel-body\">";
    }

    function footerTabelaTabela(){
        echo "<p class=\"text-right\">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>";
    }

    function trTableIncisivel($indicadorDemografico){
        echo "";
    }


    function btnExpand($indicadorDemografico){
        $idBtn = "btn_show_tabela_".$indicadorDemografico;
        if($_REQUEST['req'] != "imprimir" && $_REQUEST['req'] != "xls"){
            echo "<td>";
            echo "<button id='$idBtn' type='button' class='btn btn-primary btn-sm glyphicon glyphicon-plus' onclick=\"js_expandTabela('$indicadorDemografico');\"></button>";
            echo "</td>";
        }

    }


    function filtraTabelaIdicadoresDemograficos(array $arrayPost){

    }

    function montaTabelaSemFiltros(array $arrayRequest, Par3_Model_Ide $ide,$descrMuniOrEstad){
        $arrayIndicadores = $this->modelIndicadorDemografico->fetchAll();
        $intIniciativa = (int) $arrayRequest['iniid'];
        foreach($arrayIndicadores as $value){
            echo "<tr>";
                //echo $this->btnExpand($value['indcod']);
                echo "<td width=\"50%\" style=\"text-align: left;\" id=\"td_ide_{$value['indcod']}\">";
                    echo "<strong>{$value['indddesc']} - {$descrMuniOrEstad}</strong>";
                echo "</td>";
                echo $this->montaCheckBoxFiltro($intIniciativa,1,trim(strtoupper($value['indcod'])));
                echo $this->montaCheckBoxFiltro($intIniciativa,2,trim(strtoupper($value['indcod'])));
                echo $this->montaCheckBoxFiltro($intIniciativa,3,trim(strtoupper($value['indcod'])));
                echo "</tr>";
                echo "<tr id=\"tr_tabela_{$value['indcod']}\" style=\"display: none;\">";
                echo "<td colspan=\"5\">";
                //echo $this->montaTabela($ide,$value['indtabcorede'],trim(strtoupper($value['indcod'])),$value['indtabuf'],$value['indtabmunic']);
                echo "</td>";
                echo "</tr>";
        }
    }

    function montaTabelaComFiltros(array $arrayPost, Par3_Model_Ide $ide,$descrMuniOrEstad){
        $arrayIndicadores = array();
        $intIniciativa = (int) $arrayPost['iniid'];
        echo "<thead>";
//        if($_REQUEST['req'] != "imprimir" && $_REQUEST['req'] != "xls"){
//            echo "<th></th>";
//        }
        echo "<th>indicador</th>";
        echo "<th>Plano de trabalho</th>";
        echo "<th>Análise Técninca </th>";
        echo "<th>Análise de Mérito</th>";
        echo "</thead>";
        echo "<tbody>";
        $arrayIndicadores = $this->modelIndicadorDemografico->fetchAll();
            foreach($arrayIndicadores as $arraIndic){
                $countRegist = 0;
                    //se não existir filtro de indicadores e nem status de indicadores
                    if(!isset($arrayPost['inidesc']) && !isset($arrayPost['filterTableIndicadoresStatus'])) {
                           $this->auxiliarMontarTabelaComFiltro($arraIndic, $descrMuniOrEstad, $arrayPost, $ide);
                    }


                    //se existir filtro de indicadores
                    if(isset($arrayPost['inidesc'])){
                        //se existir filtro de indicadores e não existir filtro de status
                        if(!isset($arrayPost['filterTableIndicadoresStatus'])){
                            if (in_array(trim(strtolower($arraIndic['indcod'])), $arrayPost['inidesc'])) {
                                $this->auxiliarMontarTabelaComFiltro($arraIndic, $descrMuniOrEstad, $arrayPost, $ide);
                            }
                        }else{      //se existir filtros de status
                                    if(in_array(0,$arrayPost['filterTableIndicadoresStatus'])){
                                        $ifTrueTodos = $this->modeloIniciativaIndicadoresDemograficosStatus->indicadorComFiltroTodos($intIniciativa,$arraIndic['indcod']);
                                        if($ifTrueTodos == true && in_array(trim(strtolower($arraIndic['indcod'])), $arrayPost['inidesc'])){
                                            $this->auxiliarMontarTabelaComFiltro($arraIndic, $descrMuniOrEstad, $arrayPost, $ide);
                                        }
                                    }
                                    //se existir filtro de indicador e o filtro status não for todos
                                    else{
                                        $possuiUmDosFiltro = false;
                                        foreach($arrayPost['filterTableIndicadoresStatus'] as $value){
                                           if($this->modeloIniciativaIndicadoresDemograficosStatus->indicadorFiltraPorStatusNaoTodos($intIniciativa,$value,$arraIndic['indcod']) == true &&
                                                in_array(trim(strtolower($arraIndic['indcod'])), $arrayPost['inidesc'])){

                                               $possuiUmDosFiltro = true;
                                           }
                                        }
                                        if($possuiUmDosFiltro == true){
                                            $this->auxiliarMontarTabelaComFiltro($arraIndic, $descrMuniOrEstad, $arrayPost, $ide);
                                        }
                                    }

                        }
                    }

                    //se não existir filtro de indicadores
                    if(!isset($arrayPost['inidesc'])){
                        //se existir filtro de indicadores e não existir filtro de status
                        if(isset($arrayPost['filterTableIndicadoresStatus'])){
                            if(in_array(0,$arrayPost['filterTableIndicadoresStatus'])){
                                $ifTrueTodos = $this->modeloIniciativaIndicadoresDemograficosStatus->indicadorComFiltroTodos($intIniciativa,$arraIndic['indcod']);
                                //ver($ifTrueTodos);
                                if($ifTrueTodos == true){
                                    $this->auxiliarMontarTabelaComFiltro($arraIndic, $descrMuniOrEstad, $arrayPost, $ide);
                                }
                            }else{
                                $showIndicador = false;
                                foreach($arrayPost['filterTableIndicadoresStatus'] as $arr){
                                   if($this->modeloIniciativaIndicadoresDemograficosStatus->indicadorFiltraPorStatusNaoTodos($intIniciativa,$arr,$arraIndic['indcod']) == true){
                                       $showIndicador = true;
                                   }
                                }
                                if($showIndicador == true){
                                    $this->auxiliarMontarTabelaComFiltro($arraIndic, $descrMuniOrEstad, $arrayPost, $ide);
                                }
                            }

                        }
                    }

                $countRegist++;
                }
        echo "</tbody>";
    }

    public function alteraIndicadorDemografico(array $arrayPOST){
        $this->modeloIniciativaIndicadoresDemograficosStatus->popularDadosObjeto($arrayPOST);
        $this->modeloIniciativaIndicadoresDemograficosStatus->salvar();
        $this->modeloIniciativaIndicadoresDemograficosStatus->commit();
    }

    public function auxiliarMontarTabelaComFiltro(array $arraIndic,$descrMuniOrEstad,array $arrayPost,$ide){
        $array = $this->modeloIniciativaIndicadoresDemograficosStatus->montaArrayiIndicadoresFiltro($arrayPost);
        //var_dump($array);
        echo "<tr>";
        //echo $this->btnExpand($arraIndic['indcod']);
        echo "<td width=\"50%\" style=\"text-align: left;\" id=\"td_ide_{$arraIndic['indcod']}\">";
        echo "<strong>{$arraIndic['indddesc']} - {$descrMuniOrEstad}</strong>";
        echo "</td>";
        echo $this->montaCheckBoxFiltro($arrayPost['iniid'],1,trim(strtoupper($arraIndic['indcod'])));
        echo $this->montaCheckBoxFiltro($arrayPost['iniid'],2,trim(strtoupper($arraIndic['indcod'])));
        echo $this->montaCheckBoxFiltro($arrayPost['iniid'],3,trim(strtoupper($arraIndic['indcod'])));
        echo "</tr>";
        echo "<tr id=\"tr_tabela_{$arraIndic['indcod']}\" style=\"display: none;\">";
        echo "<td colspan=\"5\">";
        echo $this->montaTabela($ide,$arraIndic['indtabcorede'],trim(strtoupper($arraIndic['indcod'])),$arraIndic['indtabuf'],$arrayPost['indtabmunic']);
        echo "</td>";
        echo "</tr>";


    }

    public function montaCampoSelectDescricaoIndicadorByIndicativa($iniid,$id_status){
        $arrayIndicadores = $this->modeloIniciativaIndicadoresDemograficosStatus->getIndicadorePelaIniciativa($iniid,$id_status);
        if($arrayIndicadores > 0){
            $simec = new Simec_View_Helper();
            $result = array();
            foreach($arrayIndicadores as $key => $value){
                   $result[trim(strtolower($value['indcod']))] = $value['indddesc'];
                }
            echo "<div class='col-md-3'><label class=\"control-label active pull-right \">Descrição do Grupo/Indicador:</label></div><div class='col-md-4'>".$simec->select('inidesc[]', '',null, $result, array (
                'placeHolder' => '',
                'maxlengh' => 50,
                'width' => "450px;",
                'required',
                'multiple' => 'multiple',
                ))."</div>"."<div class='col-md-3'><button type=\"button\" onclick=\"ajax_getIndicadoresMontarTabela();\" class=\"btn btn-primary pull-left\" id=\"pesquisar_indicadores\" value=\"pesquisar_indicadores\"><i class=\"fa fa-search\"></i>Pesquisar</button>
                            &nbsp;&nbsp;<button type=\"reset\" class=\"btn btn-success\"  onclick=\"js_limpaIndicadoresEtabelas();\"  name=\"limpar_form_indicadores\" id=\"limpar_form_indicadores\" style=\"margin-left: 10px;\"><i class=\"fa fa-eraser\"></i> Limpar</button>
                    </div>";
            }

            else{
            echo "<div class=\"col-md-3\">
                                <label class=\"control-label active pull-right \">Descrição do Grupo/Indicador:</label>
                            </div>
                            <div class=\"col-md-4\">
                                <style>
                                    .dropdown-menu{
                                        z-index: 10000 !important;
                                    }
                                </style>

                                <div class=\"form-group \">
                                    <input class=\"form-control\" style=\"width:400px\" id=\"exampleInputName2\" placeholder=\"Selecione...\" disabled=\"disabled\" type=\"text\">
                                </div>
                            </div>
                            <div class=\"col-md-3\">
                                <button disabled type=\"submit\" class=\"btn btn-primary pull-left\" id=\"pesquisar_indicadores\" value=\"pesquisar_indicadores\"><i class=\"fa fa-search\"></i>
                                    Pesquisar</button>
                            </div>";
        }
        }

    public function montaTabelaPlanejamento(){
        $ide = new Par3_Model_Ide();
        $ide->tipo = 'estado';
        $ide->estuf = 'AC';
        echo $this->montaTabela($ide,1,"03","TbIdeTabela3UfMec","TbIdeTabela3MuMec");
    }
}














