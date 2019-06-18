<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";
include_once APPRAIZ . "par3/classes/model/AdesaoPrograma.class.inc";
include_once APPRAIZ . "www/par3/_funcoes.php";
include_once APPRAIZ . "par3/classes/controller/Processo.class.inc";
include_once APPRAIZ . "par3/classes/model/Processo.class.inc";
include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';
include_once APPRAIZ.'par3/classes/controller/WS_Servico_FNDE.class.inc';
include_once APPRAIZ . 'par3/classes/controller/WS_Sigarp_FNDE.class.inc';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$obSigarp = new Par3_Controller_WS_Sigarp_FNDE();
$obSigarp->ws_usuario = WS_USUARIO_SIGEF;
$obSigarp->ws_senha = WS_SENHA_SIGEF;

?>
<form name="form-pesquisa-termo" id="form-pesquisa-termo" class="form-horizontal" method="post" enctype="multipart/form-data">
<table>
	<tr>
		<td><input type="file" name="arquivo" id="arquivo" value=""></td>
	</tr>
	<tr>
		<td><input type="radio" name="check_tipo" id="check_tipo_r" value="R" <?php echo ($_REQUEST['check_tipo'] == 'R' ? 'checked' : '')?>>Sigarp Reformulação</td>
		<td><input type="radio" name="check_tipo" id="check_tipo_e" value="E" <?php echo ($_REQUEST['check_tipo'] == 'E' ? 'checked' : '')?>>Sigarp Envio</td>
	</tr>
	<tr>
		<td><input type="submit" name="btnteste" value="enviar"></td>
	</tr>
</table>
</form>
<?php
if( $_FILES['arquivo'] ){
    
        // VERIFICANDO CAMPO VAZIO
        if($_FILES['arquivo']['name']=="") {
            echo "<script>alert('Você deve selecionar um arquivo');history.back();</script>";
            exit;
        }
        // NOME DA FOTO
        $uploadfile = $uploaddir.$_FILES['arquivo']['name'];
        
        // FORMATO DO ARQUIVO
        $extensao = pathinfo($uploadfile, PATHINFO_EXTENSION);
        
        // VALIDANDO O FORMATO
        if ($extensao!="txt") {
            echo "<script>alert('Somente arquivos no formato $extensao');window.location.href='reformular_item_sigarp.php';</script>";
            exit;
        }
        
        // ENVIANDO A FOTO
        if(@move_uploaded_file($_FILES['arquivo']['tmp_name'], $uploadfile)) {
            echo "foi enviado: ".$uploadfile."<br>";
        }
        
        /* definimos $linha com o arquivo */
        $linhas = file($uploaddir.$_FILES['arquivo']['name']);
        /* aqui pegamos cada linha que tiver valor no txt */
        for($i = 0;$i < count($linhas);$i++) {
            /* aqui exibimos as linhas, nesse exemplo uso <br> para exibir um valor por linha */
            //echo $linhas[$i]."<br>";
            $sql = "SELECT DISTINCT p.proid, dt.dotid FROM par3.processo p
            	INNER JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'
            WHERE p.pronumeroprocesso = '".trim($linhas[$i])."'
            	AND p.prostatus = 'A'";
            $arTermo = $db->pegaLinha($sql);
            
            if( $arTermo['dotid'] ){
                $obSigarp->dotid = $arTermo['dotid'];
                
                if( $_REQUEST['check_tipo'] == 'R' ){
                    $obSigarp->reformular = 'S';
                    $arRetorno = $obSigarp->enviarItensSigarpReformulacao();
                } else {
                    $arRetorno = $obSigarp->enviarItensSigarp();
                }
                
                if( $arRetorno['erro'] == 't'  ){
                    $arRetorno['processo'] = $linha;
                    $arRetorno['dotid'] = $arTermo['dotid'];
                    $arrError[] = $arRetorno;
                }
            }
        }
        
        ver($arrError);
}



















