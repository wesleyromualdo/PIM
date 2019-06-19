<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/07/2018
 * Time: 11:09
 */

class Par3_Helper_ProgramaFPMHelper
{

    private $pfasobre; //campo textarea "Orientações Gerais:"
    private $perfil_permissao;
    private $diretorio_view = "apoio_emergencial_municipio";
    private $totalMatriculasAtendidasPDDEIntegral;      //Total de matrículas atendidas pelo PDDE Integral

    private $total_matricula_ensino_fundamental;            //Total de matrículas do Ensino Fundamental: A
    private $total_matriculas_atendidas_pdde_integral;      //Total de Matrículas atendidas pelo PDDE Integral: B
    private $total_matricula_tempo_integral_censo;          //Total de matrículas de tempo integral Censo: C
    private $total_matriculas_aptas_atendimento_c1;         //Matrículas aptas para atendimento ? critério 1: D
    private $vl_repassado_resolucao_fnde2018;               //Valor repassado pela Resolução FNDE nº 11/2018: E
    private $vl_aplicado_pnme_municipio;                    //Valor a ser aplicado no PNME pelo município: F
    private $total_matriculas_aptas_atendimento_c2;         //Matrículas aptas para atendimento ? critério 2: G
    private $total_matriculas_atendimento_pnme15h;          //Total de matrículas para atendimento no PNME 15h: H
    private $total_matriculas_distribuídas_escolas;         //Matrículas a serem distribuídas nas escolas: I
    private $dadosEscolasDaUnidade;                         //Dados das escolas por unidade
    private $btn_salvar;
    private $data_final_programa;
    private $docid_unidade;
    private $visibilidade_form;
    private $estado_documento;
    private $menor_valor_TotalMatriculaParaAtendimento;
    private $esddesc;
    private $noPeriodo;

    public function __construct(){
    }


    public function imprimirDadosDaUnidade($dadosMatricula, $dadosPorEscola){

    }




    /**
     * @return mixed
     */
    public function getNoPeriodo()
    {
        return $this->noPeriodo;
    }

    /**
     * @param mixed $noPeriodo
     */
    public function setNoPeriodo($noPeriodo)
    {
        $this->noPeriodo = $noPeriodo;
    }




    /**
     * @return float|int
     * @description Este método retorna o menos valor entre critério 01 e critério 02
     */
    public function getMenorValorTotalMatriculaParaTendimento(){
        if(!empty($this->total_matricula_ensino_fundamental) && $this->total_matricula_ensino_fundamental > 0){
            $c1 = ($this->total_matricula_ensino_fundamental - ($this->total_matriculas_atendidas_pdde_integral + $this->total_matriculas_atendimento_pnme15h));
            $c2 = ($this->vl_repassado_resolucao_fnde2018 / 120);
            if($c1 > $c2){
                return $c2;
            }else{
                return $c1;
            }
        }else{
            return 0;
        }
    }

    private function formataValor($valor,$tpFormatacao){
        $vl = "";
        switch ($tpFormatacao){
            case "F":
                $vl = str_replace(".","",$valor);
                $vl = str_replace(",",".",$vl);
                $vl = (float) $vl;
                break;
            case "I":
                $vl = str_replace(".","",$valor);
                $vl = str_replace(",","",$vl);
                $vl = (int) $vl;
                break;
        }

        return $vl;
    }


    public function getEstadoDocumento(){
        return $this->estado_documento;
    }

    public function setEstadoDocumento($estado_documento){
        return $this->estado_documento = $estado_documento;
    }

    /**
     * @param $visibilidade_form
     * @description true desabilita
     */
    public function setVisibilidadeForm($visibilidade_form){
        $this->visibilidade_form = $visibilidade_form;
    }

    public function getVisibilidadeForm(){
        return $this->visibilidade_form;
    }

    public function setDocId($docid_unidade){
        $this->docid_unidade = $docid_unidade;
    }

    public function getDocId(){
        return $this->docid_unidade;
    }

    public function setDataLimitePrograma($data_final_programa){
        $this->data_final_programa = $data_final_programa;
    }

    public function getDataLimitePrograma(){
        return $this->data_final_programa;
    }
    /**
     * @return string
     */
    public function getBtnSalvarFormulario(){
        $html = "";
        switch ($this->btn_salvar){
            case PAR3_FLUXO_FPM_NAO_INICIADO:
                $html .= "<button type=\"button\" class=\"btn btn-success\" disabled=\"disabled\">
                                    <span class=\"glyphicon glyphicon-floppy-disk\" aria-hidden=\"true\"></span> Salvar
                            </button>";
                break;

            case PAR3_FLUXO_FPM_EM_CADASTRAMENTO:
                $date = new DateTime($this->getDataLimitePrograma());
                //Caso já tenha preenchido os campos
                if($this->visibilidade_form == false){
                    $html .= "<button type=\"button\" class=\"btn btn-success\" onclick=\"js_editarValorTabelaEscolasTemCerteza();\"><span class=\"glyphicon glyphicon-pencil\" aria-hidden=\"true\"></span> Editar</button>";
                }else{ //se não houver preenchido os campos
                    $html .= "<button type=\"button\" class=\"btn btn-success\" onclick=\"js_editarValorTabelaEscolas();\"><span class=\"glyphicon glyphicon-floppy-disk\" aria-hidden=\"true\"></span> Salvar</button>";
                }
                break;

            case PAR3_FLUXO_FPM_REABERTO:
                $html .= "<button type=\"button\" class=\"btn btn-success\" onclick=\"js_editarValorTabelaEscolas();\"><span class=\"glyphicon glyphicon-floppy-disk\" aria-hidden=\"true\"></span> Salvar</button>";
                break;

            default:
                $html .= "";
                break;
        }

        return $html;
    }

    public function setBtnSalvarFormulario($esdid){
        $this->btn_salvar = $esdid;
    }

    public function getTotalMatriculasAptasAtendimentoC2(){
        if(!empty($this->vl_aplicado_pnme_municipio) && $this->vl_aplicado_pnme_municipio > 0){
            //$this->total_matriculas_aptas_atendimento_c2 = ($this->vl_repassado_resolucao_fnde2018 / $this->vl_aplicado_pnme_municipio);
            $this->total_matriculas_aptas_atendimento_c2 = ($this->vl_aplicado_pnme_municipio / 120);
        }else{
            $this->total_matriculas_aptas_atendimento_c2 = 0;
        }
        return $this->total_matriculas_aptas_atendimento_c2;
    }


    /**
     * @param $vl_aplicado_pnme_municipio F
     */
    public function setVlAplicadoPnmeMunicipio($vl_aplicado_pnme_municipio){
        $this->vl_aplicado_pnme_municipio = $vl_aplicado_pnme_municipio;
    }

    /**
     * @return mixed F
     */
    public function getVlAplicadoPnmeMunicipio(){
        return $this->vl_aplicado_pnme_municipio;
    }

    // D = ((A - C) - B)
    public function getTotalMatriculasAptasAtendimentoC1(){
        $this->total_matriculas_aptas_atendimento_c1 = (
            ($this->total_matricula_ensino_fundamental - $this->total_matricula_tempo_integral_censo) - $this->total_matriculas_atendidas_pdde_integral);
        return $this->total_matriculas_aptas_atendimento_c1;
    }


    /**
     * @param $vl_repassado_resolucao_fnde2018 E
     */
    public function setVlRepassadoResolucaoFnde2018($vl_repassado_resolucao_fnde2018){
        $this->vl_repassado_resolucao_fnde2018 = $vl_repassado_resolucao_fnde2018;
    }

    /**
     * @return mixed E
     */
    public function getVlRepassadoResolucaoFnde2018(){
        return $this->vl_repassado_resolucao_fnde2018;
    }

    /**
     * @return mixed
     */
    public function getTotalMatriculaEnsinoFundamental()
    {
        return $this->total_matricula_ensino_fundamental;
    }

    /**
     * @param mixed $total_matricula_ensino_fundamental
     */
    public function setTotalMatriculaEnsinoFundamental($total_matricula_ensino_fundamental)
    {
        $this->total_matricula_ensino_fundamental = $total_matricula_ensino_fundamental;
    }

    /**
     * @param $total_matriculas_atendidas_pdde_integral B
     */
    public function setTotalMatriculasAtendidasPddeIntegral($total_matriculas_atendidas_pdde_integral){
        $this->total_matriculas_atendidas_pdde_integral = $total_matriculas_atendidas_pdde_integral;
    }

    /**
     * @return mixed B
     */
    public function getTotalMatriculasAtendidasPddeIntegral()
    {
        return $this->total_matriculas_atendidas_pdde_integral;
    }

    /**
     * @param $total_matricula_tempo_integral_censo C
     */
    public function setTotalMatriculaTempoIntegralCenso($total_matricula_tempo_integral_censo){
        $this->total_matricula_tempo_integral_censo = $total_matricula_tempo_integral_censo;
    }

    /**
     * @return mixed C
     */
    public function getTotalMatriculaTempoIntegralCenso(){
        return $this->total_matricula_tempo_integral_censo;
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $perfil_permissao
     * @description Este método é seta a permissão do perfil para alterar alguns campos
     *
     */
    public function setPerfilPodeEditar($perfil_permissao = false){
        $this->perfil_permissao = $perfil_permissao;
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $pfasobre
     * @return string
     * @description Este método vefica se o perfil logado tem permissão para editar input "Orientações Gerais"
     *
     */
    public function inputOrientacoesGerais(){
        $pfasobre = $this->getOrientacoesGerais();
        $html = "";
        //se o perfil não tiver permissão para editar o campo Orientações Gerais
        if($this->perfil_permissao == false){
            $html = "<div class='well'>";
            $html .= "$pfasobre";
            $html .= "</div>";
        }else{
            $html .= "<textarea class='form-control pull-left summernote' name='pfasobre' rows='12' id='pfasobre'>{$pfasobre}</textarea>";
        }
        return $html;
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $pfasobre
     * @return string
     * @description Este método vefica se o perfil logado tem permissão para editar input "Orientações Gerais" e mostra em tela o botão salvar
     *
     */
    public function inputButtonSalvarOrientacoesGerais(){
        $html = "";
        if($this->perfil_permissao == true){
            $html .= "<p style='padding-top: 20px;'>
                        <button type='button' class='btn btn-success' onclick='js_salvaValorOrientacoesGerais(true);'><span class='glyphicon glyphicon-floppy-disk' aria-hidden='true'></span> Salvar</button>
                      </p>";
        }
        return $html;
    }

    public function openTab($tab){
        $js = "<script> js_openAna('{$tab}')</script>";
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $subview
     * @throws Exception
     * @description Este método inclui os arquivos responsáveis por serem renderizados por cada aba
     */
    public function includeSubView($subview){
        $directory = APPRAIZ . "par3" . DIRECTORY_SEPARATOR . "modulos" . DIRECTORY_SEPARATOR . "principal". DIRECTORY_SEPARATOR ."adesao" . DIRECTORY_SEPARATOR ."termos" . DIRECTORY_SEPARATOR . $this->diretorio_view . DIRECTORY_SEPARATOR  .$subview .".inc";
        $directory = str_replace(array('\\',"/"),DIRECTORY_SEPARATOR,$directory);

        if(file_exists($directory)){
            include_once("$directory");
        }else{
            throw new Exception("O Diretório {$directory} não existe");
        }
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $pfasobre
     * @description Este método carrega o text/html do campo Orientações Gerais
     */
    public function setOrientacoesGerais($pfasobre){
        $this->pfasobre = $pfasobre;
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @return mixed
     * @description Este método retorna o text/html do campo Orientações Gerais
     */
    public function getOrientacoesGerais(){
        return $this->pfasobre;
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $inep
     * @param null $value
     * @return string
     * @description Este método cria o botão da tabela que recebe a quantidade de vagas que será atendida
     */
    public function inputsEscola($inep,$value = null,$a,$b,$c){
        $c1 = (( $a - $b ) - $c);
        $html = "";
        $value = (!empty($value))? $value : '0';
        $value = ($this->estado_documento == PAR3_FLUXO_FPM_REABERTO) ? "0" : $value;
        $disabled = ($this->visibilidade_form == false) ? "disabled='disabled'" : "";

        if(!empty($inep)){
            //valores do campo hidden
            $idHidden = "vl_{$inep}";
            $valueHidden = (!empty($value)) ? $inep."|".$value : $inep."|"."0";
            //$html .= " <input type=\"hidden\" value=\"{$valueHidden}\"  id=\"{$idHidden}\" class=\"vlestudanteporescola\" name=\"vlestudanteporescola[]\">  \n";
            $html .= " <input type=\"text\" {$disabled} value=\"{$value}\" max='$c1' name=\"vlestudanteporescolat[]\" id=\"$inep\" onblur='js_validaValorPorEscola(this.id, this.value,$a,$b,$c)' class=\"sonumero vl_escola\" onblur=\"js_setValueHidden('{$inep}',this.value);\" >";
        }
        return $html;
    }


    public function getListaHiddenEscolaToSetValor(){
        if(count($this->dadosEscolasDaUnidade) > 0 && !empty($this->dadosEscolasDaUnidade)){
            $html = "";
            foreach($this->dadosEscolasDaUnidade as $escola){
                $value = (!empty($escola['fpmevlestudantespnme15h']))? $escola['fpmevlestudantespnme15h'] : '0';
                $value = ($this->estado_documento == PAR3_FLUXO_FPM_REABERTO) ? "0" : $value;
                $valueHidden = (!empty($escola['codinep'])) ? $escola['codinep']."|".$value : $escola['codinep']."|"."0";
                $idHidden = "vl_{$escola['codinep']}";
                $html .= " <input type=\"hidden\" value=\"{$valueHidden}\"  id=\"{$idHidden}\" class=\"vlestudanteporescola\" name=\"vlestudanteporescola[]\">  \n";
            }
        }
        return $html;
    }


    /**
     * @param $dadosEscolasDaUnidade
     */
    public function setListaEscolasDaUnidade($dadosEscolasDaUnidade){
        $this->dadosEscolasDaUnidade = $dadosEscolasDaUnidade;
    }

    /**
     * @return mixed
     */
    public function getListaEscolasDaUnidade(){
        return $this->dadosEscolasDaUnidade;
    }


    /**
     * @param $field
     */
    public function tooltip($field){
        $html = "";
        switch ($field){
            case 'A':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="left" title= "Total de matrículas do E.F. regular da rede municipal de educação de acordo com o Censo Escolar 2017"></span>';
                break;

            case 'B':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="left" title= "Total de alunos previstos nos planos de atendimento das escolas no âmbito do Programa Novo Mais Educação (adesão 2017) que receberam valores referentes à 1ª parcela do PDDE Integral."></span>';
                break;

            case 'C':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="left" title= "Total de matrículas de tempo integral no E.F. da rede municipal de educação de acordo com o Censo Escolar 2017"></span>';
                break;

            case 'D':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="left" title= "Resultado do somatório do total de matrículas atendidas pelo PDDE Integral e do total de matrículas de tempo integral Censo subtraídos do total de matrículas do ensino fundamental."></span>';
                break;

            case 'E':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="right" title= "Valor referente à transferência de recursos aos entes federativos de acordo com o Anexo I da Resolução FNDE nº 11/2018."></span>';
                break;

            case 'F':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="right" title= "Valor a ser aplicado pelos municípios no Programa Novo Mais Educação com carga horária de 15h de atividades complementares semanais. O valor não pode ser inferior a R$ 120,00 (valor médio por aluno para o período) nem superior ao valor total repassado ao município pela Resolução FNDE nº 11/2018."></span>';
                break;


            case 'G':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="right" title= "Resultado da divisão entre o valor a ser aplicado pela rede no PNME e o valor mensal unitário de R$ 120,00 médio por aluno no PNME/PDDE Integral para um período de 04 meses (R$ 120,00/aluno). Caso o resultado não seja um número inteiro, será arredondado para cima."></span>';
                break;

            case 'H':
                $html .= '<span class="glyphicon glyphicon-question-sign" aria-hidden="true" data-toggle="tooltip" data-placement="right" title= "Total de matrículas a ser atendido pelo município nos moldes do PNME com os recursos recebidos pela Resolução nº 11/2018. Será igual ao menor valor entre ?Matrículas aptas para atendimento ? critério 1? e ?Matrículas aptas para atendimento ? critério 2."></span>';
                break;
        }
        echo $html;

    }


    public function getHtmlTabelaEscolaImpressao(){
        $html = "<table class=\"table table-bordered dataTable\" border='1px'>
                    <thead>
                        <th style='font-size: 10px;'>Código INEP</th>
                        <th style='font-size: 10px;'>Nome da Escola</th>
                        <th style='font-size: 10px;'>UF</th>
                        <th style='font-size: 10px;'>Município</th>
                        <th style='font-size: 10px;'>Localização</th>
                        <th style='font-size: 10px;'>Total de matrículas do 1° ao 9° ano (Censo 2017)</th>
                        <th style='font-size: 10px;'>Matrículas PDDE Integral</th>
                        <th style='font-size: 10px;'>Matrículas de tempo integral 1º ao 9º (Censo 2017)</th>
                        <th style='font-size: 10px;'>Estudantes para atendimento no PNME 15h</th>
                    </thead>
                    <tbody>";
        if(count($this->dadosEscolasDaUnidade) > 0 && !empty($this->dadosEscolasDaUnidade)){
            foreach($this->dadosEscolasDaUnidade as $escola){
                $html .= "<tr>";
                $html .= "<td style='font-size: 10px;'>{$escola['codinep']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['escnome']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['estuf']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['mundescricao']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['tp_localizacao']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['total_matricula_censo']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['total_matricula_pdde_integral']}</td>";
                $html .= "<td style='font-size: 10px;'> {$escola['total_matricula_censo_integral']}</td>";
                $html .= "<td style='font-size: 10px;'>{$escola['fpmevlestudantespnme15h']}</td>";
                $html .= "</tr>";
            }
        }
        $html .= "</tbody>";
        $html .= "</table>";
        return $html;
    }

    public function getResulValidation($array){
        $html = "";
        if($array['erro'] == true){
            foreach($array['msg_error'] as $e){
                $html .= "{$e}<br>";
            }
        }
        echo "<script> 
                         swal({
                        title: \"<span style='font-size: 27px'>Atenção</span>\",
                        text: \"$html\",
                        showCancelButton: true,
                        confirmButtonColor: \"#DD6B55\",
                        //confirmButtonText: \"Sim, tenho certeza!\",
                        closeOnConfirm: \"on\",
                        //cancelButtonText: \"Cancelar\",
                        html: true
                     });
             
             </script>";
    }


    public function getDadosMatriculaImpressao($valorCamposMatricula,$dadosFndeDoMunicipio,$somaMatriculasAtendidas,$textoTermo){
        $a = (!empty($valorCamposMatricula['total_matricula_ensino_fundamental'])) ? $valorCamposMatricula['total_matricula_ensino_fundamental'] : '0';
        $b = (!empty($valorCamposMatricula['total_matriculas_atendidas_pdde_integral'])) ? $valorCamposMatricula['total_matriculas_atendidas_pdde_integral'] : '0';
        $c = (!empty($valorCamposMatricula['total_matricula_tempo_integral_censo'])) ? $valorCamposMatricula['total_matricula_tempo_integral_censo'] : '0';
        $d = (($a!=0)) ? ($a - ($b - $c)) : '0';
        $e = (!empty($dadosFndeDoMunicipio['vl_repassefnde2018'])) ?  $dadosFndeDoMunicipio['vl_repassefnde2018'] : '0';
        $f = (!empty($valorCamposMatricula['fpmvaloraplicadopnmemunicipio'])) ? $valorCamposMatricula['fpmvaloraplicadopnmemunicipio'] : '0';
        $g = (!empty($c) && $c > 0) ? ($c / 120) : '0';
        $g_arredondado = ceil($g);

        $a =(!empty($a)) ? number_format($a,2,",",".") : "0,0";
        $e =(!empty($e)) ? number_format($e,2,",",".") : "0,0";
        $f =(!empty($f)) ? number_format($f,2,",",".") : "0,0";


        $html = "<div style='padding: 15px;'><p>$textoTermo</p></div>";
        $html .= "<table border='1px'>";
        $html .= "<tr>";
        $html .= "<td style='font-size: 10px;'> Total de matrículas do Ensino Fundamental: {$a}</td>";
        $html .= "<td style='font-size: 10px;'> Total de Matrículas atendidas pelo PDDE Integral: {$b}</td>";
        $html .= "<td style='font-size: 10px;'> Total de matrículas de tempo integral Censo: {$c}</td>";
        $html .= "</tr>";
        $html .= "<tr>";
        $html .= "<td style='font-size: 10px;'> Valor repassado pela Resolução FNDE nº 11/2018: {$e}</td>";
        $html .= "<td style='font-size: 10px;'> Valor a ser aplicado no PNME pelo município: {$f}</td>";
        $html .= "<td style='font-size: 10px;'> Total de alunos atendidos pelo FPM: {$somaMatriculasAtendidas}</td>";
        $html .= "</tr>";
        $html .= "</table>";
        $urlImg = APPRAIZ . "/imagens/brasao.gif";
        $hrImpressao = date('d/m/Y - H:i:s');

        /*NOVO HTML*/
        $htmlFinal = "<div id=\"div-impressao\" style=\"\">
                    <div id=\"div_cabecalho\" class=\"row col-lg-offset-2\" style='margin-bottom: 50px;'>
                        <table width=\"92%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"text-align:center;\">
                            <tr bgcolor=\"#ffffff\">
                                <td nowrap align=\"left\" valign=\"middle\" height=\"1\" style=\"padding:5px 0 0 0;\">
                                    MEC/FNDE <br>
                                    Impressão Fomento à execução do Programa Novo Mais Educação<br>
                                </td>
                                <td align=\"right\" valign=\"middle\" height=\"1\" style=\"padding:5px 0 0 0;\">
                                    Impresso por: <b> {$_SESSION['usunome']}
                                    </b>
                                    <br/>
                                    Hora da Impressão: {$hrImpressao}<br/>
                                </td>
                            </tr>
                        </table>
                       
                    </div>
                    <div class=\"clearfix\" style=\"margin-bottom:10px;\"></div>
                    <div id=\"impressao-content\">
                            {$html}
                    </div>
                </div>";
        return $htmlFinal;
    }

}