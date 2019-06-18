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


$atividades = $rel->getAtividadesCronograma(false);

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

        table,table th, table tr, table td{border: 1px solid black; padding-left: 15px; padding-right: 15px;}
        table {border-collapse:collapse;}
        nav.navbar-listagem{display:none!important}
        .modal{display:none}
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

    table,table th, table tr, table td{border: 1px solid black; padding-left: 15px; padding-right: 15px;}
    table {border-collapse:collapse; width: 100% !important;}
    nav.navbar-listagem{display:none!important}
    .modal{display:none}
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

        <div class="content maiusculo col-lg-offset-2 col-lg-8" style="margin-bottom: 380px;>
            <p>Estado {$estado}</p>
        </div>

        <div class="content maiusculo col-lg-offset-2 col-lg-8">
            <p>Relatório dos resultados de elaboração/adequação dos planos de carreira e remuneração no estado de {$estado} no período de {$diaIni} à {$diaFim} de {$mes} de {$ano}</p>
        </div>

        <div class="content italico col-lg-offset-2 col-lg-8">
            <div>{$avaliador}</div>
            <div>{$d}</div>
        </div>

    </div>

    <div class="page col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h4>APRESENTAÇÃO</h4>
            <p>Com o intuito de assessorar os entes federativos nos processos de elaboração/adequação dos planos de carreira e remuneração dos profissionais da educação escolar básica pública, cumprindo dessa forma com o que preceitua a legislação, a Secretaria de Articulação com os Sistemas de Ensino ? SASE constituiu uma Rede de Assistência Técnica. O desenho pactuado entre MEC, CONSED e UNDIME para essa Rede compreende uma equipe composta por um coordenador por estado, um técnico para cada 50 municípios e equipes de supervisão para estados com mais de 102 municípios.</p>
            <p>A participação dos entes federativos nessa Rede se dará por adesão. Para tanto, os secretários estaduais/municipais de educação comunicam ao coordenador estadual, e este, formaliza `a SASE/MEC.</p>
            <p>A Rede não substitui os espaços de diálogo instituídos nos estados e municípios para debater a valorização dos profissionais da educação escolar básica pública, porém sua estratégia de trabalho fortalecerá as ações colaborativas nas instâncias da Federação.</p>
            <p>
                Este plano de Trabalho tratará:
                <ul>
                    <li>das articulações desenvolvidas junto aos estados e municípios para que façam adesão à Rede de Assistência Técnica;</li>
                    <li>das formações dos AEs para prestar assessoria aos entes federativos; e</li>
                    <li>da elaboração de relatórios das ações executadas.</li>
                </ul>
            </p>
        </div>

        <div class="content col-lg-offset-2 col-lg-8">
            <h4>JUSTIFICATIVA</h4>
            <p>A valorização dos profissionais da educação escolar básica, pela sua relevância, é um princípio estabelecido pela Constituição Federal do Brasil de 1988, especificamente, em seu artigo 206, incisos V e VIII. Esse princípio foi reforçado por outras legislações constando, por exemplo, no artigo nº 67 da Lei 9394/1996, que trata das Diretrizes e Bases da Educação Brasileira (LDB).</p>
            <p>Em 16 de julho de 2008 foi sancionada a Lei n° 11.738, que instituiu o piso salarial profissional nacional para os profissionais do magistério público da educação básica (PSPN), regulamentando o que se encontra disposto na Constituição e na LDB.</p>
            <p>Com aprovação do Plano Nacional de Educação (PNE), Lei nº 13.005, de 25 de junho de 2014, ficou assegurado, através da Meta nº 18, a obrigatoriedade da União, estados e municípios garantirem planos de carreira para os profissionais da educação básica e superior pública, ampliando a abrangência que a Lei 11.738/2008 havia estabelecido para os profissionais que, necessariamente, precisam ter suas carreiras regulamentadas, tendo como referência de vencimento inicial o Piso Salarial Profissional Nacional (PSPN).</p>
            <p>Considerando a necessidade dos entes federativos se adequarem ao que estabelece a Legislação, o Ministério da Educação (MEC), o Conselho Nacional de Secretários de Educação (CONSED) e a União Nacional dos Dirigentes Municipais de Educação (UNDIME) construiriam uma Rede de Assistência Técnica. A principal atribuição da Rede será o desenvolvimento de atividades de formação, junto aos gestores das redes públicas de ensino e as equipes responsáveis por processos de construção, revisão e implantação de planos de carreira e remuneração.</p>
        </div>
    </div>
    <div class="col-lg-12">
        <div class="content col-lg-offset-2 col-lg-8">
            <h4>OBJETIVO</h4>
            <p>Prestar assistência técnica aos entes federativos, voltada para a elaboração/adequação dos planos de carreira e remuneração dos profissionais da educação escolar básica pública, considerando a legislação vigente, a valorização profissional e a sua viabilidade financeira.</p>
            <h4>OBJETIVOS ESPECÍFICO</h4>
            <ul>
                <li><p>acompanhar junto ao estado e municípios a constituição das equipes responsáveis pela gestão dos planos de carreira e remuneração;</p></li>
                <li><p>capacitar as equipes responsáveis pela gestão dos planos de carreira e remuneração para a realização do diagnóstico da Rede de Ensino do ente federativo;</p></li>
                <li><p>instrumentalizar as equipes gestoras para a realização do trabalho de elaboração/adequação dos planos de carreira e remuneração dos profissionais da educação escolar básica pública, em consonância;</p></li>
                <li><p>subsidiar tecnicamente as equipes gestoras na utilização do Sistema de Apoio a Gestão de Plano de Carreira e Remuneração - SISPCR, como fundamento para a organização de uma proposta de trabalho de elaboração/adequação dos planos carreira e remuneração dos profissionais da educação escolar básica pública, partindo-se da estruturação do diagnóstico do estado e dos municípios adidos ao trabalho de assistência técnica;</p></li>
                <li><p>elaborar relatório contendo informações sobre as ações executadas.</p></li>
            </ul>
        </div>

        <div class="content col-lg-offset-2 col-lg-8">
            <h4>PÚBLICO ALVO</h4>
            <p><div class="campo-check"></div>Secretarias Municipais da Educação</p>
            <p><div class="campo-check"></div>Secretaria Estadual de Educação</p>
            <p><div class="campo-check"></div>Equipes Responsáveis pela Gestão do PCR</p>
            <p><div class="campo-check"></div>Entidades Representativas dos Profissionais da Educação</p>
            <p>Outros:____________________________________________________</p>
        </div>

        <div class="content col-lg-offset-2 col-lg-8">
            <h4>METODOLOGIA</h4>
            <p>A metodologia tratará da descrição de cada uma das etapas, adequadas à demanda que tratam os respectivos documentos técnicos, com vistas à elaboração/adequação dos planos de carreira e remuneração dos profissionais da educação escolar básica pública, apontando as estratégias e dispositivos que serão utilizados em cada uma das etapas.</p>
            <div class="borda">
                <p>Para que o trabalho aconteça com o desempenho desejado será constituída uma equipe responsável pela gestão dos planos de carreira e remuneração em cada município. Nesse sentido serão executadas ações, coordenadas pelos AEs - Avaliadores Educacionais /SASE-MEC, em parceria com a SEDUC/UF e UNDIME-SECCIONAL/UF. Destacamos algumas:</p>
                <p>
                    <ul>
                        <li>Realizaçao de reunião técnica com a equipe da SEDUC/UF e UNDIME para apresentar o plano de trabalho com o objetivo de articular o processo de adesão e garantir financiamento para encontros presenciais de formação/capacitação/ das equipes de gestão para elaboração/adequação dos PCR nos Municípios.</li>
                        <li>Apresentar aos gestores de educação o planejamento da Formação dos AEs para orientar os entes federativos na adequação/elaboração do PCR dos profissionais da educação escolar básica pública (contendo, no mínimo, os objetivos, justificativa, metodologia, participantes e a programação).</li>
                        <li>Consolidação de relatórios que deverão ser encaminhados a DIVAPE/SASE/MEC.</li>
                        <li>Todos serão convidados para agregar em torno de único objetivo: Pensar políticas de valorização dos profissionais da educação para a melhoria da educação pública do Estado da(e) _______, por meio do Plano de Carreira e Remuneração dos Profissionais da Educação Básica Pública.</li>
                    </ul>
                </p>
            </div>
        </div>
        <div class="col-lg-12">
            <div class="content col-lg-offset-2 col-lg-8">
                <h5>ATIVIDADES (CRONOGRAMA)</h5>
                <div>{$atividades}</div>
            </div>
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
echo $contents;
//echo $html;
?>
