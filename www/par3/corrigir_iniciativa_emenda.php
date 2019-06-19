<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

$data_created = date("c");
echo '02';

try{
    $arqXml = <<<XML
    <?xml version="1.0" encoding="ISO-8859-1"?>
    <request>
    	<header>
    		<app>SIGARP</app>
    		<version>1.0</version>
    		<created>$data_created</created>
    	</header>
    	<body>
    		<auth>
    			<usuario>USAP_WS_SIGARP</usuario>
    			<senha>62856723</senha>
    		</auth>
    		<params>
    			<nu_seq_item>3785</nu_seq_item>
    			<uf>PE</uf>
    		</params>
    	</body>
    </request>
XML;
    
    $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/pregao';
    
    $xml = Fnde_Webservice_Client::CreateRequest()
    ->setURL($urlWS)
    ->setParams(array('xml' => $arqXml, 'method' => 'listar'))
    ->setConnectTimeOut(100)
    ->setTimeOut(100)
    ->execute();
    
    $xmlRetorno = $xml;
    
    $xml = simplexml_load_string(stripslashes($xml));
    ver($urlWS, $xml,d);
} catch (Exception $e) {
    # Erro 404 página not found
    if ($e->getCode() == 404) {
    }
    $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
    $erroMSG = str_replace("'", '"', $erroMSG);
    ver($urlWS, $erroMSG,d);
}
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

function insereArquivo( $sql ){
    if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
        $nomeArquivo 		= 'corrigir_iniciativa_emenda_'.date('Ymd');
        $diretorio		 	= APPRAIZ . 'arquivos/par';
        $diretorioArquivo 	= APPRAIZ . 'arquivos/par/'.$nomeArquivo.'.txt';
        
        if( !is_dir($diretorio) ){
            mkdir($diretorio, 0777);
        }
        
        $fp = fopen($diretorioArquivo, "a+");
        if ($fp) {
            stream_set_write_buffer($fp, 0);
            fwrite($fp, $sql);
            fclose($fp);
        }
    }
}

if( $_REQUEST['ineid'] ){
    $sql = "UPDATE par3.iniciativa_emenda SET inestatus = 'I' WHERE ineid = {$_REQUEST['ineid']};
            UPDATE par3.iniciativa_emenda_item_composicao SET ieistatus = 'I' WHERE ineid = {$_REQUEST['ineid']};";
    $db->executar($sql);
    $db->commit();
    insereArquivo($sql);
    echo "<script>
            alert('Operação realizada com sucesso');
            window.location.href = 'corrigir_iniciativa_emenda.php';
        </script>";
    die;
}

if( $_REQUEST['ieiid'] ){
    $sql = "UPDATE par3.iniciativa_emenda_item_composicao SET ieistatus = 'I' WHERE ieiid = {$_REQUEST['ieiid']};";
    $db->executar($sql);
    $db->commit();
    insereArquivo($sql);
    echo "<script>
            alert('Operação realizada com sucesso');
            window.location.href = 'corrigir_iniciativa_emenda.php?duplicado=S';
        </script>";
    die;
}

if( $_REQUEST['duplicado'] == 'S' ){
    $sql = "SELECT DISTINCT inpid
            FROM par3.iniciativa_emenda ie
            WHERE ie.ineid IN (SELECT ineid FROM par3.iniciativa_emenda_item_composicao WHERE ieistatus = 'A')
            	AND inestatus = 'I'
            ORDER BY inpid";
    $arrEmenda = $db->carregar($sql);
    $arrEmenda = $arrEmenda ? $arrEmenda : array();
    
    $html = '<table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
    $html .= '<tr>';
    $html .= '<th>ineid</th>';
    $html .= '<th>inpid</th>';
    $html .= '<th>edeid</th>';
    $html .= '<th>emeid</th>';
    $html .= '<th>inevalor</th>';
    $html .= '<th>inecontrapartida</th>';
    $html .= '<th>Estados</th>';
    $html .= '<th></th>';
    $html .= '</tr>';
    foreach ($arrEmenda as $key => $v) {
        $sql = "SELECT ineid, inpid, edeid, emeid, inevalor, inestatus, docid, ineresponsavel, inecontrapartida
                FROM par3.iniciativa_emenda ie WHERE ie.inpid = {$v['inpid']} AND inestatus = 'A' ORDER BY ineid, edeid, inpid;";
        $arrIniciativa = $db->carregar($sql);
        $arrIniciativa = $arrIniciativa ? $arrIniciativa : array();
        
        foreach ($arrIniciativa as $ini) {
            $sql = "SELECT ieiid, ineid, ipiid, ieiquantidade,
                        (SELECT inestatus FROM par3.iniciativa_emenda WHERE ineid = c.ineid) as status
                    FROM par3.iniciativa_emenda_item_composicao c WHERE ineid IN (SELECT ineid FROM par3.iniciativa_emenda ie WHERE ie.inpid = {$v['inpid']}) AND ieistatus = 'A' ORDER BY ineid, ipiid";
            $arrComposicao = $db->carregar($sql);
            $arrComposicao = $arrComposicao ? $arrComposicao : array();
            
            $html .= '<tr>
                        <td><a href=corrigir_iniciativa_emenda.php?ineid='.$ini['ineid'].'>'.$ini['ineid'].'</a></td>
                        <td>'.$ini['inpid'].'</td>
                        <td>'.$ini['edeid'].'</td>
                        <td>'.$ini['emeid'].'</td>
                        <td>'.simec_number_format( $ini['inevalor'], 2, ',', '.' ).'</td>
                        <td>'.simec_number_format( $ini['inecontrapartida'], 2, ',', '.' ).'</td>
                        <td>'.$ini['esddsc'].'</td>
                        <td>
                            <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">
                                <tr>
                                    <th>ieiid</th>
                                    <th>ineid</th>
                                    <th>ipiid</th>
                                    <th>ieiquantidade</th>
                                    <th>Status</th>
                                </tr>';
            foreach ($arrComposicao as $com) {
                $cor = '';
                $link = $com['ieiid'];
                if( $ini['ineid'] != $com['ineid'] ){
                    $cor = ' style="color: red;" ';
                    $link = '<a href=corrigir_iniciativa_emenda.php?duplicado=S&ieiid='.$com['ieiid'].'>'.$com['ieiid'].'</a>';
                }
                $html .= '<tr '.$cor.'>
                                                  <td>'.$link.'</td>
                                                  <td>'.$com['ineid'].'</td>
                                                  <td>'.$com['ipiid'].'</td>
                                                  <td>'.$com['ieiquantidade'].'</td>
                                                  <td>'.$com['status'].'</td>
                                              </tr>';
            }
            $html .= '      </table>
                        </td>
                    </tr>';
        }
    }
    $html .= '</table>';
    echo $html;
    
} else {
    $sql = "SELECT count(ineid), inpid, edeid, emeid FROM par3.iniciativa_emenda WHERE inestatus = 'A'
            GROUP BY inpid, edeid, emeid
            HAVING count(ineid) > 1";
    $arrEmenda = $db->carregar($sql);
    $arrEmenda = $arrEmenda ? $arrEmenda : array();
    
    $html = '<table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
    $html .= '<tr>';
    $html .= '<th>ineid</th>';
    $html .= '<th>inpid</th>';
    $html .= '<th>edeid</th>';
    $html .= '<th>emeid</th>';
    $html .= '<th>inevalor</th>';
    $html .= '<th>inecontrapartida</th>';
    $html .= '<th>Estados</th>';
    $html .= '<th></th>';
    $html .= '</tr>';
    foreach ($arrEmenda as $key => $v) {
        
        $sql = "SELECT ineid, inpid, edeid, emeid, inevalor, inecontrapartida, e.esddsc
                FROM par3.iniciativa_emenda i
                	INNER JOIN workflow.documento d ON d.docid = i.docid
                	INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
    		    WHERE i.inpid = {$v['inpid']} AND i.edeid = {$v['edeid']} AND i.emeid = {$v['emeid']}
                    AND i.inestatus = 'A'";
        $arrIniciativa = $db->carregar($sql);
        
        foreach ($arrIniciativa as $ini) {
            $sql = "SELECT ieiid, ipiid, ieiquantidade FROM par3.iniciativa_emenda_item_composicao WHERE ineid = {$ini['ineid']} AND ieistatus = 'A' ORDER BY ipiid";
            $arrComposicao = $db->carregar($sql);
            $arrComposicao = $arrComposicao ? $arrComposicao : array();
            
            $html .= '<tr>
                        <td><a href=corrigir_iniciativa_emenda.php?ineid='.$ini['ineid'].'>'.$ini['ineid'].'</a></td>
                        <td>'.$ini['inpid'].'</td>
                        <td>'.$ini['edeid'].'</td>
                        <td>'.$ini['emeid'].'</td>
                        <td>'.simec_number_format( $ini['inevalor'], 2, ',', '.' ).'</td>
                        <td>'.simec_number_format( $ini['inecontrapartida'], 2, ',', '.' ).'</td>
                        <td>'.$ini['esddsc'].'</td>
                        <td>
                            <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">
                                <tr>
                                    <th>ieiid</th>
                                    <th>ipiid</th>
                                    <th>ieiquantidade</th>
                                </tr>';
                                foreach ($arrComposicao as $com) {
                                    $html .= '<tr>
                                                  <td>'.$com['ieiid'].'</td>
                                                  <td>'.$com['ipiid'].'</td>
                                                  <td>'.$com['ieiquantidade'].'</td>
                                              </tr>';
                                }
            $html .= '      </table>
                        </td>
                    </tr>';
        }
    }
    $html .= '</table>';
    echo $html;
}