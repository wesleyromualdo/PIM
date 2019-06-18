<?php
header('Content-type: text/html; charset="utf-8"',true);


$acessoRapidoCabecalhoIncluido = true;
if ( in_array($_SESSION['sisid'], array('283','284' )) )
{

    include_once 'cabecalho_bootstrap_v4.inc';
}else if ($_SESSION['sislayoutbootstrap'] == 't') 
{
    include_once 'cabecalho_bootstrap.inc';
}
 
else if ($_SESSION['sislayoutbootstrap'] == 'zimec')
{
    include_once 'cabecalho_bootstrap_v3.inc';
}
else
{


    $theme = "verde";
    $_SESSION["theme_simec"] = $theme;


    if (isset($_POST["theme_simec"]) && $_SESSION["theme_simec"] != $_POST["theme_simec"]) {
        $theme = $_POST["theme_simec"] ? $_POST["theme_simec"] : 'areia';
        setcookie("theme_simec", $theme , time()+60*60*24*30, "/");
        $_SESSION["theme_simec"] = $theme;
        gravaThemaUsuario($theme);
    }
    
    if($_REQUEST['carregarcolaboradores'] and ( $_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma'] )) { 
        $sql = "select distinct
                u.usucpf as codigo,
                u.usunome as descricao
                from seguranca.usuario u
                inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
                inner join seguranca.perfil p on p.pflcod = pf.pflcod and   p.sisid = " . $_SESSION['sisid'] . "
                inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = p.sisid
                where us.suscod = 'A' and p.pflnivel >= (
                select min(pflnivel) from seguranca.perfil 
                inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod 
                where perfilusuario.usucpf = '" . $_SESSION['usucpforigem'] . "'
                and perfil.sisid = " . $_SESSION['sisid'] . " ) 
                order by 2 ";
        $opt = $db->carregar($sql);
        if($opt[0]) {
            foreach($opt as $op) {
                echo $op['codigo']."##".$op['descricao']."&&";
            }
        }
        exit;
    }

    $cabecalho_sistema = true;

    if($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't'){
        $sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from ". $_SESSION['sisdiretorio'] .".programacaoexercicio order by 1";
        $arrAnoExercicio = $db->carregar($sql);
    }
    
   if (! $_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't')
   {
       $sql = sprintf(
            "SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
            $_SESSION['sisdiretorio'],
            $_SESSION['exercicio']
        );
        $_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
   }
    // altera o ano de exercício (caso o usuário solicite)
    if ( $_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio'] ) {
        if($arrAnoExercicio){
            foreach($arrAnoExercicio as $anoExercicio){
                $arrAno[] = $anoExercicio['codigo'];
                if($anoExercicio['prsexerccorrente'] == "t"){
                    $ano_corrente = $anoExercicio['codigo'] ;
                }
            }
        }
        if(is_array($arrAno)){
            if(in_array($_REQUEST['exercicio'],$arrAno)){
                $_SESSION['exercicio'] = $_REQUEST['exercicio'];  
            }else{
                $_SESSION['exercicio'] = $ano_corrente;
            }
        }
        $exercicio = $_SESSION['exercicio'];
        
        $sql = sprintf(
            "SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
            $_SESSION['sisdiretorio'],
            $_REQUEST['exercicio']
        );
        $_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
    }

    include APPRAIZ . "includes/registraracesso.php";
?>
<?php 
    include APPRAIZ . "includes/cabecalho_abaPainel.php";
?>
<html>
    <head>
        <title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
        <meta content="IE=9" http-equiv="X-UA-Compatible" />
                
        <!-- Bootstrap CSS -->
        <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
        <script src="../includes/JQuery/jquery-1.5.1.min.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="../includes/libs_jquery/chosen.js/docsupport/prism.js" type="text/javascript"></script>
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen" >
        <center>
            <div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;">
                <br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br>
            </div>
        </center>

        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">
        <link href="../includes/Estilo.css" rel="stylesheet" type="text/css"/>
        <link href='../includes/listagem.css' rel='stylesheet' type='text/css'/>

        <!--Inclue do arquivo que carrega o CSS da pagina Inicio do Obras-->
        <link rel="stylesheet" type="text/css" media="screen, print" href="../includes/layoutNovoObras2.css">

        <!-- INCLUSÃO DO CSS PARA NOVO LAYOUT DO SIMEC - 15/05/10 -->
        <?php if(is_file( APPRAIZ."www/includes/layout/".$theme."/include_simec.inc" )) include APPRAIZ."www/includes/layout/".$theme."/include_simec.inc"; ?>

        <script language="JavaScript">
            function setpfl() {
                document.getElementById('setperfil2').submit();
            }

            function setpfl1() {
                document.getElementById('setperfil').submit();
            }

            function abrirsistema( sisid ) {
                location.href = "../muda_sistema.php?sisid=" + sisid;
            }

            function abrir_popup_mensagem()
            {
                w = window.open( '../geral/popup_mensagem.php', 'mensagens', 'width=780,height=400,scrollbars=yes,menubar=no,toolbar=no,statusbar=no' );
                w.focus();
            }

            function changeSystem(system)
            {
                location.href = "../muda_sistema.php?sisid=" + system;
            }
            function exibeThemas()
            {
                var div = document.getElementById('menu_theme');
                if (div.style.display == 'none')
                    div.style.display = '';
                else
                    div.style.display = 'none';
            }

            function alteraTema(){
                document.getElementById('formTheme').submit();
            }
        </script>

        <!-- AGRUPADOR -->
        <script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>

        <?php include APPRAIZ . "includes/gera_menu.inc"; ?>
        <?php (IS_PRODUCAO ? require_once APPRAIZ . 'includes/google_analytics.php' : ''); ?>
    </head>
    <?php
        $sql = sprintf("SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']);
        $paginic = $db->pegaUm( $sql );
    ?>
    <body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

    <?php include_once APPRAIZ . 'includes/cabecalho_mostramsg.php'; ?>
    <?php include_once APPRAIZ . 'includes/cabecalho_css.php'; ?>
    




    <table border="0" id="main" cellpadding="0" cellspacing="0" style="width: 100%;">
        <tr>
            <td align="left" valign="top" colspan="2" >

            <div id="top_content" class="notprint">
                <br />
                <div id="top_content_logo_menu">
                    <div style="float: left; padding: 5px;">
                        <img style="cursor:pointer" onclick="window.location.href='<?=$_SESSION['sisarquivo']?>.php?modulo=<?=$paginic?>'" src="../includes/layout/<? echo $theme ?>/img/logo_sp_horizontal.png" >
                    </div>
                    
                    <?php $sistemas = carregarSistemas(); ?>
                    <div style="float: left;">
                        <div style="margin-top: 10px; margin-left: 15px;" class="cabecalho">
                            <select data-placeholder="Escolha um módulo do sistema..." id="cabecalho" class="chosen-select" style="width:200px;" tabindex="2" onchange="javascipt:changeSystem(this.value);">
                                <?php if ($sistemas && is_array($sistemas)) : ?>
                                   <?php foreach ($sistemas as $sistema): ?>
                                        <option <?php if($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                    <?php endforeach; ?>
                                <?php endif; ?>
                            </select>
                        </div>
                    </div>
                </div>

                <script type="text/javascript">
                    var jq = jQuery.noConflict();
                    jq('.chosen-select').chosen({allow_single_deselect:true});
                </script>

                <div id="top_content_sistema">

                <div id="div-dados-login" >
                    <?php if ($_SESSION["sisexercicio"] == 't'): ?>
                    <?php if ($_SESSION['exercicio']) $exercicio = $_SESSION['exercicio']; ?>
                    <?php $exerc = $_SESSION['sisdiretorio'] == 'monitora' ? 'Exercício' : 'Exercício'; ?>
                    <?php endif; ?>
                      
                        <table id="content_top_simula">
                            <tr>
                                <td>
                                    <?= $exerc ? $exerc." :" : "" ?>
                                </td>
                                <td style="color:#FFFFFF" >
                                    <form id="setperfil" name="setperfil" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
                                        <?php $exerc ? $db->monta_combo('exercicio',$arrAnoExercicio,'S','','setpfl1','') : ""; ?>
                                    </form>
                                </td>

                                <td align="right">

                                    <a href="javascript:self.print();"><img src="../includes/layout/<? echo $theme ?>/img/bt_print.png" style="vertical-align: middle" border="0" /></a>
                                    <a href="javascript:DoPopup('<?= $_SESSION['sisarquivo'] ?>.php?modulo=ajuda/ajuda&acao=C&mnuid=<?= $modulo_atual ?>','helpsimec',440,550);"><img style="vertical-align: middle" src="../includes/layout/<? echo $theme ?>/img/bt_help.png" border="0" id="bt_help_sistema"/></a>
                                    <a href="javascript:abrir_popup_mensagem();"><img src="../includes/layout/<? echo $theme ?>/img/bt_mail.png" style="vertical-align: middle" border="0" /></a>
                                    <a href="/logout.php"><img src="../includes/layout/<? echo $theme ?>/img/bt_logoff.png" border="0" style="vertical-align: middle" /></a>

                                    <div style="display:none;margin-left:-150px" id="menu_theme">
                                        <form id="formTheme" action="" method="post" >
                                            Tema:
                                            <select class="select_ylw" name="theme_simec" title="Tema do SIMEC" onchange="alteraTema(this.value)" >
                                                <?php include(APPRAIZ."www/listaTemas.php") ?>
                                            </select>
                                        </form>
                                    </div>

                                </td>
                            </tr>
                            <form id="setperfil2" name="setperfil2" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
                                <tr>
                                    <td style="white-space: no-wrap;font-weight:bold; color: #FFFFff" >
                                        Usuário:
                                    </td>
                                    <td colspan="2" style="white-space: no-wrap;font-weight:bold" >
                                        <table border="0" cellpadding="0" cellspacing="0">
                                            <tr>
                                                <td style="white-space: no-wrap;font-weight:bold" valing="top">
                                                    <?php $_SESSION['usucpf'] = $_REQUEST['usucpf_simu'] ? $_REQUEST['usucpf_simu'] : $_SESSION['usucpf']; ?>
                                                    <?php if ( $_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma'] ): ?>
                                                    <?php
                                                        $sql = "select distinct
                                                                       u.usucpf as codigo,
                                                                       u.usunome as descricao
                                                                  from seguranca.usuario u
                                                            inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
                                                            inner join seguranca.perfil p on p.pflcod = pf.pflcod and p.sisid = " . $_SESSION['sisid'] . "
                                                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = p.sisid
                                                                 where us.suscod = 'A' 
                                                                   and u.usucpf='".$_SESSION['usucpf']."'
                                                              order by 2";
                                                        
                                                        $_SESSION['usuuma'] = 1;
                                                        
                                                        if (isset($_SESSION['dadosusuario'][$_SESSION['usucpf']]))
                                                        {
                                                            $dadosusuario = $_SESSION['dadosusuario'][$_SESSION['usucpf']];
                                                        }
                                                        else
                                                        {
                                                            $dadosusuario = $db->pegaLinha($sql);
                                                            $_SESSION['dadosusuario'][$_SESSION['usucpf']] = $dadosusuario;
                                                        }   
                                                        
                                                        $usucpf_simu = $_SESSION['usucpf'];
                                                    ?>
                                                        <span onclick="javascript:simularUsuario();" title="Simular Usuário" style="cursor: pointer; color: #FFFFff"><?php echo $dadosusuario['descricao']; ?></span>
                                                        <input type="hidden" name="usucpf_simu" id="usrs" value="<?php echo $usucpf_simu;?>">
                                                    <?php else: ?>
                                                    <?php echo $_SESSION['usunome']; ?>
                                                    <?php endif; ?>
                                                </td>
                                                <td style="width: 30px; text-align: center;">
                                                    <?php if ($_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma']) : ?>
                                                        <img alt="" src="../imagens/simular_usuario.png" title="Simular Usuário" onclick="javascript:simularUsuario();" style="width: 18px; cursor: pointer;">
                                                    <?php endif; ?>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                </tr>
                                <tr>
                                    <td></td>
                                    <td colspan=2 style="white-space: no-wrap;">
                                        <div class="layBarraCronometroEstilo" id="cronometro_div" style="color: #FFFFff">Sua sessão expira em:<span id="cronometro" style="font-weight:bold; color: #f00;"></span></div>
                                    </td>
                                </tr>
                        </table>
                    </form>
                </div>

                <div id="top_content_label_sistema">
                        <?php $arrSis = $sistemas[$_SESSION['sisid']]; ?>
                        <h2 style='text-align: center; color: #FFFFff;'><?php echo $arrSis['sisabrev'] ?></h2>
                </div>
            </div>
            <div style="clear: both;"></div>
       </div>
            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen"  style="border-bottom: 1px solid;" >
                <tr bgcolor="#ffffff">
                    <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
                    <td height="20" nowrap>
                        SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
                        Ministério da Educação / SE - Secretaria Executiva<br>
                        DTI - Diretoria de Tecnologia da Informação<br>
                    </td>
                    <td height="20" align="right">
                        Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
                        Órgão: <?= $_SESSION['usuorgao']; ?><br>
                        Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
                    </td>
                </tr>
                <tr bgcolor="#ffffff">
                    <td colspan="2">&nbsp;</td>
                </tr>
            </table>
            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notprint" style="display: none">
                <tr>
                    <td valign="bottom">
                        <?php $sistemas = carregarSistemas(); ?>
                        <?php if ( $sistemas ) : ?>

                        <?php endif; ?>
                    </td>
                </tr>
                <tr>
                    <td height="31px" class="submenu" ></td>
                </tr>
            </table>
            <table class="tbl_conteudo" width="100%" border="0" cellpadding="0" cellspacing="0">
                <tr>
                    <td align="right" height="17" valign="middle" ></td>
                </tr>
                <tr>
                    <td valign="top"  style="padding-bottom:15px;" >
                        <?php if ($modulo_atual != "") : ?>
                        <?php   
                            $sql = sprintf("select distinct mnu1.mnudsc as menu1  , mnu2.mnudsc as menu2 , mnu3.mnudsc as menu3 , mnu1.mnulink as mnulink1  , mnu2.mnulink as mnulink2 , mnu3.mnulink as mnulink3    from seguranca.menu mnu1, seguranca.menu mnu2,  seguranca.menu mnu3 where mnu3.mnuidpai=mnu2.mnuid and mnu2.mnuidpai=mnu1.mnuid and mnu3.mnuid = %d", $modulo_atual);

                            if (isset($_SESSION['rastro'][$modulo_atual]))
                            {
                                $rastro = $_SESSION['rastro'][$modulo_atual];
                            }
                            else 
                            {
                                $rastro = $db->pegaLinha($sql);
                                $_SESSION['rastro'][$modulo_atual] = $rastro;
                            }

                            if ($rastro): 
                        ?>
                            <div class="row" style="margin-top: 10px;">
                                <div class="col-md-12">
                                    <div class="view">
                                        <ul class="breadcrumb">
                                            <li>
                                                <?php if($rastro['menulink1']): ?>
                                                    <a <?php if($url == $rastro['menulink1']) echo 'class="active"'; echo "href='{$rastro['menulink1']}'" ;?> contenteditable="true"><?php echo $rastro['menu1'] ?></a>
                                                <?php else: ?>
                                                    <?php echo $rastro['menu1'] ?>
                                                <?php endif ?>
                                            </li>
                                            <li>
                                                <?php if($rastro['menulink2']): ?>
                                                    <a <?php if($url == $rastro['menulink2']) echo 'class="active"'; echo "href='{$rastro['menulink2']}'" ;?> contenteditable="true"><?php echo $rastro['menu2'] ?></a>
                                                <?php else: ?>
                                                    <?php echo $rastro['menu2'] ?>
                                                <?php endif ?>
                                            </li>
                                            <li>
                                                <?php if($rastro['menulink3']): ?>
                                                    <a <?php if($url == $rastro['menulink3']) echo 'class="active"'; echo "href='{$rastro['menulink3']}'" ;?> contenteditable="true"><?php echo $rastro['menu3'] ?></a>
                                                <?php else: ?>
                                                    <?php echo $rastro['menu3'] ?>
                                                <?php endif ?>
                                                <span class="divider"></span>
                                            </li>
                                        </ul>
                                <?php endif; ?>
                                    </div>
                                </div>
                            </div>
<?php endif; ?>
<?php } ?>
<?php
    if (!IS_PRODUCAO):
        $host = $_SERVER['HTTP_HOST'];

        if(false !== strpos($host, 'simec-d')){
            $labelAmbiente = 'Ambiente de Desenvolvimento';
            $classCorAmbiente = 'barraAmbienteDivPrincipalCorFundoVerde';
        } else {
            $labelAmbiente = 'Ambiente Local';
            $classCorAmbiente = 'barraAmbienteDivPrincipalCorFundoAzul';
        }

        if($_SESSION['baselogin'] == 'simec_espelho_producao'){
            $labelBanco = 'Espelho';
            $classCorLabelBanco = 'barraAmbienteLabelBancoEspelho';
        } else {
            $labelBanco = 'Desenvolvimento';
            $classCorLabelBanco = 'barraAmbienteLabelBancoDesenv';
        }
        $labelBanco = $_SESSION['baselogin'] == 'simec_espelho_producao'? 'Espelho': 'Desenvolvimento';

        $complemento = IS_TREINAMENTO ? 'style="color: yellow; background-color: red;"' : '';
?>
    <div id="divAmbiente" style="display: none;">
        <div class="barraAmbienteDivPrincipal <?php echo $classCorAmbiente; ?>" <?php echo $complemento; ?>>
            <table class="barraAmbienteTabela">
                <tr>
                    <?php if (IS_TREINAMENTO): ?>
                        <td style="text-align: left;">
                            <div class="barraAmbienteLabelBanco"  style="color: yellow; background-color: red; text-align: center; font-weight: normal;">
                                ATENÇÃO! Este ambiente é destinado para TREINAMENTO do uso do sistema SIMEC.
                            </div>
                        </td>
                    <?php else: ?>
                        <td style="text-align: left;">
                            <span class="barraAmbienteTexto">
                                <?php echo $labelAmbiente ?>
                            </span>
                        </td>
                        <td style="text-align: right;">
                            <span class="barraAmbienteTexto">
                                Banco de dados:
                                    <span class="barraAmbienteLabelBanco <?php echo $classCorLabelBanco; ?>">
                                        <?php echo $labelBanco; ?>(<?php echo $nome_bd; ?>)
                                    </span>
                            </span>
                        </td>
                    <?php endif ?>

                </tr>
            </table>
        </div>
    </div>

<?php endif; ?>
<?php if ($_SESSION['usucpf'] != $_SESSION['usucpforigem']):?>
    <div id="" class="avisoSimulandoUsuario">
        <img src="/imagens/exclama.gif"> Você está simulando um usuário e por isso <b>NÃO conseguirá salvar os dados</b>.
    </div>


 <!--JavaScript cabecalho_js.php-->
<?php 
    include APPRAIZ . "includes/cabecalho_js.php";
 ?>


<?php endif; 
    include_once 'bloqueio_prestacao_contas_par_par3_obras2/verificabloqueio.inc';
?>