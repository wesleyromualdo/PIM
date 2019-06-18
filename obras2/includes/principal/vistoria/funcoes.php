<?php /* ****arquivo vazio**** */


function salvarArquivosArt(){
    global $db;

    $obrid = $_REQUEST['obrid'];

    $art_exec = NULL;
    $art_fisc = NULL;

    //1 - ExecuÃ§Ã£o
    $campos	= array("obrid"	  => $obrid,
        "oardesc" => "'".$_POST['oardesc1']."'",
        "tpaid"   => $_POST['tpaid1'],
        "oardata" => " now() ");

    $file = new FilesSimec("obras_arquivos", $campos, 'obras2');
    if($_FILES["arquivo1"]){
        $arquivoSalvo = $file->setUpload($_POST['oardesc1'],"arquivo1", TRUE, "oarid");
        if($arquivoSalvo){
            $arqid_exec = $file->getIdArquivo();

            $oarid = $file->getCampoRetorno();
            $art_exec = TRUE;

            $arrArquivosArt['usucpf'] = $_POST['usucpf_fiscal'];
            $arrArquivosArt['entidempresa']= $_POST['empresa_contratada_id'];
            if(empty($arrArquivosArt['entidempresa'])){$arrArquivosArt['entidempresa'] = null;}
            //ver('aaa',d);
            $arrArquivosArt['oarid'] = $oarid;
            $FiscalregAtividade                 = new FiscalRegistroAtividade();
            $FiscalregAtividade->popularDadosObjeto($arrArquivosArt);
            $FiscalregAtividade->salvar();
        }else{
            $arqid_exec = NULL;
            $art_exec = FALSE;
        }
    }

    unset($file);
    unset($campos);
    unset($arquivoSalvo);

    //2 - FiscalizaÃ§Ã£o
    $campos	= array("obrid"	  => $obrid,
        "oardesc" => "'".$_POST['oardesc2']."'",
        "tpaid"   => $_POST['tpaid2'],
        "oardata" => " now() ");

    $file = new FilesSimec("obras_arquivos", $campos, 'obras2');
    if($_FILES["arquivo2"]){
        $arquivoSalvo = $file->setUpload($_POST['oardesc2'],"arquivo2", TRUE, "oarid");
        if($arquivoSalvo){
            $arqid_fisc = $file->getIdArquivo();
            $oarid = $file->getCampoRetorno();
            $art_fisc   = TRUE;

            $arrArquivosArt['usucpf'] = $_POST['usucpf_fiscal'];
            $arrArquivosArt['entidempresa']= $_POST['empresa_contratada_id'];

            if(empty($arrArquivosArt['entidempresa'])){$arrArquivosArt['entidempresa'] = null;}
            $arrArquivosArt['oarid'] = $oarid;
            $FiscalregAtividade                 = new FiscalRegistroAtividade();
            $FiscalregAtividade->popularDadosObjeto($arrArquivosArt);
            $FiscalregAtividade->salvar();
        }else{
            $arqid_fisc = NULL;
            $art_fisc   = FALSE;
        }
    }

    //Registro de Atividades
    $arrArquivosArt['arquivo_art_exec'] = $arqid_exec;
    $arrArquivosArt['arquivo_art_fisc'] = $arqid_fisc;



    $arrArquivosArt['arquivo_art_fisc'] = $arqid_fisc;
    $arrArquivosArt['arquivo_art_fisc'] = $arqid_fisc;

    $supervisao = new Supervisao();
    $supervisao->registroDeMudancaDeArquivoArt($arrArquivosArt, $obrid);
    $supervisao->commit();

    if($art_exec == TRUE && $art_fisc == TRUE){
        echo '<script type="text/javascript">
                        alert("Arquivos anexados com sucesso.");
                        document.location.href = document.location.href;
                  </script>';
    }elseif($art_exec == FALSE && $art_fisc == TRUE){
        echo '<script type="text/javascript">
                        alert("Erro ao fazer o upload do arquivo ART de ExecuÃ§Ã£o. Arquivo ART de FiscalizaÃ§Ã£o anexado com sucesso.");
                        document.location.href = document.location.href;
                  </script>';
    }elseif($art_exec == TRUE && $art_fisc == FALSE){
        echo '<script type="text/javascript">
                        alert("Erro ao fazer o upload do arquivo ART de FiscalizaÃ§Ã£o. Arquivo ART de ExecuÃ§Ã£o anexado com sucesso.");
                        document.location.href = document.location.href;
                  </script>';
    }
}

function downloadArquivosArt(){

    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid( $_REQUEST['arqid'] );
    $eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo( $_REQUEST['arqid'] );

    die('<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>');
}

function excluirArquivosArt(){

    $obrid = $_REQUEST['obrid'];

    if(possui_perfil(Array(PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE))){
        echo "<script type=\"text/javascript\">
                    alert('VocÃª nÃ£o tem permissÃµes suficientes para excluir esse documento!');
                    document.location.href = document.location.href;
              </script>";
    }else{

        global $db;

        $obrasArquivos	= new ObrasArquivos( $_REQUEST['oarid'] );

        if($obrasArquivos->tpaid == 25){
            $arrArquivosArt['arquivo_art_exec'] = $obrasArquivos->arqid;
            $arrArquivosArt['arquivo_art_fisc'] = NULL;
        }elseif($obrasArquivos->tpaid == 26){
            $arrArquivosArt['arquivo_art_exec'] = NULL;
            $arrArquivosArt['arquivo_art_fisc'] = $obrasArquivos->arqid;
        }

        $supervisao = new Supervisao();
        $supervisao->registroDeMudancaDeArquivoArt($arrArquivosArt, $obrid);

        $obrasArquivos->popularDadosObjeto( Array('oarstatus' => 'I') )
            ->salvar();
        $obrasArquivos->commit();

        echo "<script type=\"text/javascript\">
			alert('OperaÃ§Ã£o realizada com sucesso!');
			document.location.href = document.location.href;
		  </script>";
    }
}



function getSqlDadosObraVinculada($obrid)
{
    return <<<SQL
    SELECT o.obrperccontratoanterior,
                     (SELECT obrpercentultvistoria FROM obras2.obras WHERE obrid = o.obridvinculado) as obrpercentultvistoria_vinculado,
                      o.obridvinculado
               FROM obras2.obras as o
               WHERE obrid = {$obrid}
SQL;
}


function getSqlDadosFiscal($empid) {
    return <<<SQL
    SELECT
            u.usunome, u.usucpf
        FROM obras2.usuarioresponsabilidade ur
        INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
        INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND sisid = 147 AND us.susstatus = 'A' AND us.suscod = 'A'
        INNER JOIN obras2.empreendimento e ON e.empid = {$empid}
        WHERE
            ur.rpustatus = 'A' AND
            u.suscod = 'A' AND
            ur.pflcod = 948 AND
            
            ( (ur.empid = {$empid}) )
SQL;

}