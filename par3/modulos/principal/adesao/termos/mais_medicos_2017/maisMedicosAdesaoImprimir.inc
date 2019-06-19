<?php include_once 'programas/_funcoes_maismedicos.php';
ob_clean();
global $db;

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$muncod = $controleUnidade->pegarMuncod($_REQUEST['inuid']);

$modelProadesao = new Par3_Model_Proadesao();
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($_REQUEST['prgid']);

if ($_REQUEST['requisicao'] == 'requisicao') {
$_POST['pfaid'] = $proadesaoDados['pfaid'];
$_POST['inuid'] = $_REQUEST['inuid'];

salvarAdesao($_POST);
exit();
}


#pega o nome do municipio
$municipio = $controleUnidade->pegarMunicipio($_REQUEST['inuid']);
$muncod = $controleUnidade->pegarMuncod($_REQUEST['inuid']);
$prgid = $_REQUEST['prgid'];
$estado = $db->pegaUm("select estuf from territorios.municipio  where muncod = '{$muncod}'");

$adpid = $db->pegaUm($sql);
$m = recuperaDadosPrefeitura();

$gestor_sus = recuperaGestorSusMunicipio();


$aderiu = verificarAdesao();
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

$modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::PREFEITO;
$objPessoaFisica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($_REQUEST['inuid'], $tenid);

$prefeito = $objPessoaFisica->arAtributos;

$dados_getor_s = buscaDadosGestorMaisMedicos('S', $muncod, $prgid);
$dados_getor_m = buscaDadosGestorMaisMedicos('M', $muncod, $prgid);
$dados_salvos = $db->pegaUm("SELECT adpdataresposta FROM par3.prodesaoprograma WHERE inuid = {$_REQUEST['inuid']} and adpano = {$_SESSION['exercicio']}");
$dadosSalvosMes = substr($dados_salvos,5,2);

function meses($meses){

switch ($meses) {
case "01":    $mes = 'Janeiro';     break;
case "02":    $mes = 'Fevereiro';   break;
case "03":    $mes = 'Março';       break;
case "04":    $mes = 'Abril';       break;
case "05":    $mes = 'Maio';        break;
case "06":    $mes = 'Junho';       break;
case "07":    $mes = 'Julho';       break;
case "08":    $mes = 'Agosto';      break;
case "09":    $mes = 'Setembro';    break;
case "10":    $mes = 'Outubro';     break;
case "11":    $mes = 'Novembro';    break;
case "12":    $mes = 'Dezembro';    break;
}

echo $mes;
}

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<html>
<head>
    <title>Mais Médicos - Termo de Adesão</title>
    <style>
        @media print {
            .notprint {
                display: none;
            }

            .div_rolagem {
                display: none;
            }
        }

        .div_rolagem {
            overflow-x: auto;
            overflow-y: auto;
            height: 50px;
        }

        .notscreen {
            display: none;
        }

        .bordaarredonda {
            background: #FFFFFF;
            color: #000;
            border: #000 1px solid;
            padding: 10px;
            -moz-border-radius: 10px 10px;
            -webkit-border-radius: 10px 10px;
            border-radius: 10px 10px;
            width: 95%;
            text-align: left;
        }

        .quebra {
            page-break-after: always !important;
            height: 0px;
            clear: both;
        }
    </style> <?php (IS_PRODUCAO ? require_once APPRAIZ . 'includes/google_analytics.php' : ''); ?>
</head>
<body>
<table border="0" width="85%" cellspacing="1" cellpadding="5" border="0" align="center">
    <tr>
        <td>
            <table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                <tr bgcolor="#ffffff">
                    <td valign="top" align="center">
                        <p align="center" style="margin-bottom: 0px;">
                            <font face="Arial, sans-serif" size="3">
                                <b>TERMO DE ADESÃO</b>
                            </font>
                        </p><br>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
</table>
<br>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
    <tr>
        <td style="font-size: 12px; font-family:arial;">
            <table align="center" border="0" cellspacing="1" cellpadding="3" width="85%">
                <tr>
                    <td align="center">

                        <p>TERMO DE ADESÃO QUE ENTRE SI CELEBRAM A SECRETARIA DE REGULAÇÃO E SUPERVISÃO DA
                            EDUCAÇÃO SUPERIOR DO MINISTÉRIO DA EDUCAÇÃO E O MUNICÍPIO DE
                            <b> <?php echo strtoupper($municipio); ?> </b>, NO
                            ESTADO DO <b><?php echo strtoupper($estado); ?> </b>, PARA DISPONIBILIZAÇÃO DA ESTRUTURA
                            DE EQUIPAMENTOS
                            PÚBLICOS, CENÁRIOS DE ATENÇÃO NA REDE E PROGRAMAS DE SAÚDE NECESSÁRIOS À AUTORIZAÇÃO
                            DE FUNCIONAMENTO DE CURSOS DE MEDICINA.</p>
                        <p>O MINISTÉRIO DA EDUCAÇÃO, CNPJ nº 00.394.445/0001-01, neste ato representado por HENRIQUE SARTORI DE ALMEIDA PRADO, Secretário de Regulação e
                            Supervisão da Educação Superior, com endereço na Esplanada dos Ministérios, Bloco "L", 1º andar, sala 100 - CEP 70.047-900, Brasília (DF), e o
                            MUNICÍPIO DE <b> <?php echo strtoupper($municipio); ?> </b> /
                            <b> <?php echo strtoupper($estado); ?> </b>, (endereço,
                            CNPJ), neste ato representado por
                            prefeito <?php echo strtoupper($prefeito['entnome']); ?>, nos termos do Art.
                            3º, I, da Lei nº 12.871, de 22 de outubro de 2013, resolvem celebrar o presente
                            Termo de Adesão para implantação e funcionamento de cursos de medicina, por
                            instituição de educação superior privada, mediante as cláusulas e condições
                            seguintes:</p>
                        <p>1. CLÁUSULA PRIMEIRA - DO OBJETO</p>
                        <p>1.1. O presente termo tem por objeto a adesão do Município de
                            <b><?php echo strtoupper($municipio); ?></b> / <b><?php echo strtoupper($estado); ?></b>
                            ao Chamamento Público previsto no Edital nº 2, de 7 de dezembro de 2017 e definição de obrigações e responsabilidades do Município no oferecimento
                            de estrutura de equipamentos públicos, cenários de atenção na rede e programas de saúde necessários à autorização de funcionamento de curso de
                            graduação em medicina a ser ofertado por instituição de educação superior privada nos termos do art. 3º, inciso IV, da Lei nº 12.871/2013.
                        </p>
                        <p>2. CLÁUSULA SEGUNDA - DAS OBRIGAÇÕES DO MUNICÍPIO</p>
                        <p>2.1. Para consecução do objeto estabelecido neste Termo de Adesão, o Município deverá atender aos seguintes critérios relativos à estrutura de
                            equipamentos públicos, cenários de atenção na rede e programas de saúde existentes e disponíveis no município e na região de saúde, previstos no
                            artigo 2° da Portaria Normativa nº 18, de 7 de dezembro de 2017, publicada no Diário Oficial da União de 8 de dezembro de 2017, e no item 2.2 do
                            Edital nº 2, de 2017, além de outros que podem ser estabelecidos pela SERES:</p>
                        <p>a) possuir número de leitos do Sistema Único de Saúde ? SUS disponíveis por aluno maior ou igual a 5 (cinco), tendo em vista a abertura de turmas com, no mínimo, 50 (cinquenta) alunos;</p>
                        <p>b) possuir Equipes Multiprofissionais de Atenção Domiciliar ? EMAD;</p>
                        <p>c) possuir o número mínimo de 17 (dezessete) Equipes de Atenção Básica - EAB;</p>
                        <p>d) possuir leitos de urgência e emergência ou Pronto Socorro;</p>
                        <p>e) possuir, pelo menos, 3 (três) Programas de Residência Médica nas especialidades prioritárias referidas nos art. 6º, incisos I e II da Lei nº 12.871 de 2013, preferencialmente em Medicina Geral de Família e Comunidade;</p>
                        <p>f) ter aderido ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica ? PMAQ, do Ministério da Saúde;</p>
                        <p>g) possuir Centro de Atenção Psicossocial - CAPS; e</p>
                        <p>h) possuir hospital com 80 (oitenta) leitos SUS, exclusivos para o curso de medicina a ser implantado, com potencial para ser certificado como hospital de ensino, conforme legislação de regência.</p>

                        <p>3. CLÁUSULA TERCEIRA - DAS OBRIGAÇÕES DA SECRETARIA DE REGULAÇÃO E SUPERVISÃO DO
                            MINISTÉRIO DA EDUCAÇÃO</p>
                        <p>3.1. Constituem obrigações da SERES:</p>
                        <p>a) selecionar os municípios para implantação e funcionamento de cursos de medicina, por instituições de educação superior privadas;</p>
                        <p>b) celebrar Termo de Adesão com o município selecionado, acompanhar e monitorar a implantação do curso de Medicina naquela localidade; </p>
                        <p>c) selecionar as instituições de educação superior privadas para oferta de cursos de graduação em Medicina nos municípios selecionados;</p>
                        <p>d) editar normas complementares necessárias ao cumprimento do estabelecido neste Termo de Adesão.</p>
                        <p>4. CLÁUSULA QUARTA - DA VIGÊNCIA</p>
                        <p>4.1. Os compromissos assumidos pelo Município no presente Termo de Adesão estão vinculados à regularidade da oferta do curso de medicina pela instituição de educação superior.</p>
                        <p>5. CLÁUSULA QUINTA - DA RESCISÃO</p>
                        <p>5.1. No caso de rescisão do presente Termo, cumpre ao Município informar à instituição de educação superior privada ofertante do curso e à Secretaria de Regulação e Supervisão da Educação Superior, com antecedência mínima de 90 (noventa) dias, a fim de preservar a continuidade da oferta do curso.</p>
                        <p>6. CLÁUSULA SEXTA - DA PUBLICAÇÃO</p>
                        <p>O presente Termo de Adesão estará disponível no Sistema de Monitoramento, Execução e Controle do MEC (http://simec.mec.gov.br). </p>
                        <p>7. CLÁUSULA SÉTIMA - DAS ALTERAÇÕES</p>
                        <p>7.1. As eventuais alterações do presente Termo de Adesão serão realizadas por meio de termo aditivo acordado entre os partícipes.</p>
                        <p>8. CLÁUSULA NONA - DA SOLUÇÃO DE CONTROVÉRSIAS</p>
                        <p>8.1. Eventual controvérsia surgida durante a execução do presente Termo de Adesão poderá ser dirimida administrativamente entre os partícipes ou, em seguida, perante a Câmara de Conciliação e Arbitragem da Administração Federal da Advocacia - Geral da União e, se inviável, posteriormente perante o foro da Justiça Federal - Seção Judiciária do Distrito Federal.</p>
                        <br/>

                        <?php if ($dados_salvos) { ?>
                            <center>Brasília-DF, <?php echo substr($dados_salvos, 8, 2); ?>
                                de <?php echo meses($dadosSalvosMes); ?>
                                de <?php echo substr($dados_salvos, 0, 4); ?></center>
                        <?php } ?>

                        <br/>
                        <center>____________________________________</center>
                        <center>HENRIQUE SARTORI DE ALMEIDA PRADO</center>
                        <center>SECRETÁRIO DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR</center>
                        <br/>
                        <center>____________________________________</center>
                        <center><?php echo strtoupper($prefeito['entnome']); ?></center>
                        <center>PREFEITO MUNICIPAL DE <b><?php echo strtoupper($municipio); ?> </b> /
                            <b> <?php echo strtoupper($estado); ?> </b></center>
                        <br/>
                        <center>_____________________________________________________</center>
                        <center><?php echo strtoupper($dados_getor_s['dapnome']); ?></center>
                        <center>GESTOR LOCAL DO SISTEMA ÚNICO DE SAÚDE</center>
                        <br/>
                    </td>
                </tr>
                <tr>
                    <td><p style="text-align: right;">&nbsp;</p></td>
                </tr>
            </table>
        </td>
    </tr>
</table>
<br>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
    <tr>
        <td class="div_rolagem" style="text-align: center;">
            <input type="button" name="impressao" id="impressao" value="Imprimir" onclick="window.print();"/>
            <input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();"/>
        </td>
    </tr>
</table>
</body>
</html>

<?php die(); ?>