<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 06/10/2015
 * Time: 08:41
 */

include APPRAIZ . 'includes/cabecalho.inc';
ob_clean ();
global $db;
include_once APPRAIZ . 'sase/classes/AvaliadorEducacional.class.inc';
include_once APPRAIZ . 'sase/classes/RelatorioAvaliadorRedeae.class.inc';
include_once APPRAIZ . "includes/classes/dateTime.inc";
$data = new Data();

$ravid = $_REQUEST['ravid'];
$rel = new RelatorioAvaliadorRedeae();
$rel->carregarPorId($ravid);
$ave = new AvaliadorEducacional();
$ave->carregarPorId($rel->aveid);

$estado = $rel->pegaEstadoAvaliador();
$avaliador = $ave->avenome;
$sql = "select mundescricao from territorios.municipio where muncod = '".$ave->muncod."'";
$cidade = $db->pegaUm($sql, 'mundescricao');
$uf = $rel->pegaEstadoAvaliador('estuf');
$dataA = date('d/m/Y');
$diaIni = $data->formataData($rel->ratdata1periodo, 'DD');
$diaFim = $data->formataData($rel->ratdata2periodo, 'DD');
$mes = $data->formataData($rel->ratdata2periodo, 'mesTextual');
$ano = $data->formataData($rel->ratdata2periodo, 'YYYY');
$d = $cidade.'/'.$uf.', '.$dataA;


$atividades = $rel->getListaAtividadesDesenvolvidas(false, false, PFLCOD_SASE_TECNICO_DIVAPE, true, null, 2);
$resultados = $rel->getResultadoConsolidadoPCP();
$quadroAssistido = $rel->getMunicipiosAssistidos();
$quadro3 = $rel->getMunComAlteracao();
$quadro4 = $rel->getMunSemAlteracao(false);
$quadro5 = $rel->getSituacaoPlanoCarreiraProfessor(false);
$quadro6 = $rel->getComprometimento(false, false);
$quadro7 = $rel->getEstruturaPlanoCarreiraRemuneracao(false, false);
$quadro8 = $rel->getMunSemInformacao(false);
$quadro9 = $rel->getAcoesDesenvMun(false);
$quadro10 = $rel->ravinfcomplementar;
$quadro11 = $rel->getAnexos(false);

$html = <<<HTML
<style>
    @media print {
        .page {
            page-break-after: always;

            min-height: 20px;
            /*background-color: #00516e;*/
        }

        .content{
            min-height: 40px;
            margin-bottom: 20px;
            /*background-color: #0079a1;*/
        }
        .content p {
            text-align: justify;
            text-justify: inter-word;
        }
        .content h4{margin-bottom: 5px;}
        .maiusculo{text-transform: uppercase}
        .minusculo{text-transform: lowercase}
        .italico{font-style: italic}

        #divInfoComp {width: 100%; border: 1px solid #000; height: 200px;}
        #tb_resultadoconsolidado th {font-size: 7px;}
        #tb_resultadoconsolidado tr {font-size: 10px;}
        table,table th, table tr, table td{border: 1px solid black; padding-left: 15px; padding-right: 15px;}
        table {border-collapse:collapse; width: 100% !important;}
        nav.navbar-listagem{display:none!important}
        .modal{display:none}
        #tbSituaAtualMun{font-size: 9px;}
        table tr td { padding: 5px; }
        table { font-size: 9px; }
        table #marid{
            font-size: 15px;
        }
        .text-center{text-align: center;}
    }
    .page {
        page-break-after: always;

        min-height: 20px;
        /*background-color: #00516e;*/
    }

    .content{
        min-height: 40px;
        margin-bottom: 20px;
        /*background-color: #0079a1;*/
    }
    .content p {
        text-align: justify;
        text-justify: inter-word;
    }
    .content h4{margin-bottom: 5px;}
    .maiusculo{text-transform: uppercase}
    .minusculo{text-transform: lowercase}
    .italico{font-style: italic}

    #divInfoComp {width: 100%; border: 1px solid #000; height: 200px;}
    #tb_resultadoconsolidado th {font-size: 7px;}
    #tb_resultadoconsolidado tr {font-size: 10px;}
    table,table th, table tr, table td{border: 1px solid black; padding-left: 15px; padding-right: 15px;}
    table {border-collapse:collapse; width: 100% !important;}
    nav.navbar-listagem{display:none!important}
    .modal{display:none}
    #tbSituaAtualMun{font-size: 9px;}
    table tr td { padding: 5px; }
    table { font-size: 9px; }
        #marid{
            font-size: 15px !important;
        }
    .text-center{text-align: center;}
</style>



<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<div class="row">
    <div class="page col-lg-12">
        <div class="content maiusculo col-lg-offset-2 col-lg-8" style="margin-bottom: 380px;">
            <div class="text-center col-lg-12">Ministério da educação</div>
            <div class="text-center col-lg-12">Secretaria de articulação com os sistemas de ensino</div>
            <div class="text-center col-lg-12">Diretoria de valorização dos profissionais de educação</div>
        </div>
        <div class="content maiusculo col-lg-offset-2 col-lg-8">
            <p>Relatório dos resultados de elaboração/adequação dos planos de carreira e remuneração no estado de {$estado} no período de {$diaIni} à {$diaFim} de {$mes} de {$ano}</p>
            Estado {$estado} <br>
            
        </div>
             <div class="content col-lg-offset-2 col-lg-8">
            {$quadroAssistido} 
        </div>
        <div class="content italico col-lg-offset-2 col-lg-8">
            <br><br><br><br><br><br>
            <div>{$avaliador}</div>
            <div>{$d}</div>
        </div>
    </div>           
            
    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h4>SUMÁRIO</h4>
            <br>Apresentação<br>
            Justificativa<br>
            Objetivo Geral<br>
            Objetivos Específicos<br>
            Atividades desenvolvidas<br>
            Resultados consolidados alcançados<br>
            Informações complementares sobre o período avaliado<br>
            Anexos<br>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h4>APRESENTAÇÃO</h4>
            <p>Este documento traz o relatório quinzenal das atividades de assistência técnica realizadas em municípios que desenvolveram/desenvolvem ações de adequaçao e/ou elaboração de planos de carreira e remuneração, considerando a legislação vigente.</p>
        </div>

        <div class="content col-lg-offset-2 col-lg-8">
            <h4>JUSTIFICATIVA</h4>
            <p>A valorização dos profissionais da educação escolar básica, pela sua relevância, é um princípio estabelecido pela Constituição Federal do Brasil de 1988, especificamente, em seu artigo 206, incisos V e VIII. Esse princípio foi reforçado por outras legislações constando, por exemplo, no artigo nº 67 da Lei 9394/1996, que trata das Diretrizes e Bases da Educação Brasileira (LDB).</p>
            <p>Em 16 de julho de 2008 foi sancionada a Lei n° 11.738, que instituiu o piso salarial profissional nacional para os profissionais do magistério público da educação básica (PSPN), regulamentando o que se encontra disposto na Constituição e na LDB.</p>
            <p>Com aprovação do Plano Nacional de Educação (PNE), Lei nº 13.005, de 25 de junho de 2014, ficou assegurado, através da Meta nº 18, a obrigatoriedade da União, estados e municípios garantirem planos de carreira para os profissionais da educação básica e superior pública, ampliando a abrangência que a Lei 11.738/2008 havia estabelecido para os profissionais que, necessariamente, precisam ter suas carreiras regulamentadas, tendo como referência de vencimento inicial o Piso Salarial Profissional Nacional (PSPN).</p>
            <p>Considerando a necessidade dos entes federativos se adequarem ao que estabelece a Legislação, o Ministério da Educação (MEC), o Conselho Nacional de Secretários de Educação (CONSED) e a União Nacional dos Dirigentes Municipais de Educação (UNDIME) construiriam uma Rede de Assistência Técnica. A principal atribuição da Rede será o desenvolvimento de atividades de formação, junto aos gestores das redes públicas de ensino e as equipes responsáveis por processos de construção, revisão e implantação de planos de carreira e remuneração. O presente relatório apresenta as ações e resultados obtidos, nos municípios assistidos pelos profissionais que compõem a mencionada Rede.</p>
        </div>

        <div class="content col-lg-offset-2 col-lg-8">
            <h4>OBJETIVO GERAL</h4>
            <p>Apresentar relatório quinzenal/mensal das atividades de assistência técnica voltadas para a adequação e/ou elaboração de planos de carreira e remuneração, desenvolvida nos municípios assistidos, considerando: a valorização profissional e a viabilidade financeira do PCR, o cumprimento da Lei 11.738/2008 e a efetivação da Meta 18 do Plano Nacional de Educação, Lei 13.005/2014.</p>
            <h4>OBJETIVOS ESPECÍFICO</h4>
            <ul>
                <li><p>Informar as ações de assistência técnica para a adequação e elaboração de planos de carreira e remuneração, desenvolvidas nos municípios assistidos no período de (xx) a (xx) de (mês) de (ano).</p></li>
                <li><p>Consolidar os resultados consolidados, decorrentes do processo de assistência técnica para adequação e elaboração de planos de carreira, desenvolvido junto aos municípios.</p></li>
                <li><p>Informar os resultados consolidados, decorrentes do processo de assistência técnica para adequação e elaboração de planos de carreira, desenvolvido junto aos municípios.</p></li>
            </ul>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h4>ATIVIDADES DESENVOLVIDAS PELO TÉCNICO(A)</h4>
            <h5>Quadro 01. Atividades executadas no período</h5>
            <div>{$atividades}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h4>RESULTADOS CONSOLIDADOS ALCANÇADOS NO PERÍODO</h4>
            <h5>Quadro 02. Informações a respeito dos Planos de Carreira e Remuneração Municipais.</h5>
            <div>{$resultados}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 03. Municipios do Estado que apresentaram alteração na sua etapa de trabalho no periodo.</h5>
            <div>{$quadro3}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 04. Municípios assistidos que permaneceram na mesma etapa do período anterior.</h5>
            <div>{$quadro4}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 05. Situação atual dos municípios assistidos com relação à existência de planos de carreira e remuneração, o cumprimento da Lei 11.738/2008 e a adequação dos planos de carreira ao que estabelece a Meta 18 do PNE.</h5>
            <div>{$quadro5}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 06. Comprometimento das receitas com gastos com pessoal (FUNDEB)</h5>
            <div>{$quadro6}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 07. Estrutura do plano de carreira e remuneração</h5>
            <div>{$quadro7}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 08. Municípios ainda sem informaçao ou sem trabalho iniciado.</h5>
            <div>{$quadro8}</div>
        </div>
    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 09. Ações a serem desenvolvidas nos municípios sem informação, sem Comissão instituída e sem alteração da etapa de trabalho.</h5>
            <div>{$quadro9}</div>
        </div>
    </div>

    <div class="col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 10. Informações complementares sobre o período avaliado.</h5>
            <div id="divInfoComp">
                {$quadro10}
            </div>
        </div>
    </div>

    <div class="col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h5>Quadro 11. Anexos</h5>
            <div>{$quadro11}</div>
        </div>
    </div>
</div>
HTML;

$content = http_build_query(array(
    'conteudoHtml' => ($html)
));

$context = stream_context_create(array(
    'http' => array (
        'method' => 'POST',
        'content' => $content
    )
));

$contents = file_get_contents ( 'http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
header ( 'Content-Type: application/pdf' );
header ( 'Content-Disposition: attachment; filename=Relatorio_AE_Técnico_PDC_'.$avaliador);
//header ( 'Content-Disposition: attachment; filename=Relatorio_AE_Técnico_PDC_'.$avaliador.'.pdf');
echo $contents;
//echo $html;
?>
