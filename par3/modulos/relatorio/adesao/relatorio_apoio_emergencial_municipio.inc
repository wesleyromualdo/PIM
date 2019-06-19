<?php
set_time_limit(0);
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 24/07/2018
 * Time: 18:35
 */


include_once APPRAIZ . 'par3/classes/controller/ProgramaFPMMunicipio.class.inc';
include_once APPRAIZ . 'par3/classes/model/ProgramaFPMMunicipioEscola.class.inc';
include_once APPRAIZ . 'par3/classes/controller/ProgramaFPMMunicipio.class.inc';

switch ($_REQUEST['requisicao']) {
    case 'exportar':
        ob_clean();
        //$lista = $adesaoPrograma->listaTermoAdesaoPnfcd($_REQUEST, 'data');
        $fpmController = new Par3_Controller_ProgramaFPMMunicipio();
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $listagem->setCabecalho(array("UF","Cód. IBGE","Município","Sit. Adesão","Sit. Plano","Matrículas Total","Matrículas PDDE Integral","Matrículas Integral Censo 2017 (3º ao 9º ano)","Matrículas Aptas 1","Valor repassado Res. 11","Valor a ser aplicado no PNME","Matrículas Aptas 2","Total de matrículas para atendimento no PNME 15h","N° de Escolas Atendidas"));
        header("Content-Type: application/vnd.ms-excel; charset=utf-8");
        header("Content-Disposition: attachment; filename=PNFCD.xls");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private",false);
        require('apoio_emergencial_municipio_export.inc');
        die;
        break;
}
require_once APPRAIZ . 'includes/cabecalho.inc';
?>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Relatório de Apoio emergencial aos municípios do FPM</h2>
            </div>
        </div>
    </div>

    <div class="ibox-content" style="height:600px;">
        <div class="row">
            <div class="col-md-12">
                <div class="col-md-6 col-md-offset-5">
                    <button type="button" class="btn btn-primary" onclick="js_emiteRelatorioXLS();">
                        <span class="glyphicon glyphicon-list-alt" aria-hidden="true"></span> Gerar planilha XLS
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>


<script>
    function js_emiteRelatorioXLS(){
        window.location = "par3.php?modulo=relatorio/adesao/relatorio_apoio_emergencial_municipio&acao=A&requisicao=exportar";
    }
</script>
