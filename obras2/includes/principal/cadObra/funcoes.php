<?php /* ****arquivo vazio**** */


function habilitaDesbloqueio($esfera)
{
    global $db, $obridVinculado;
    $obrid = ($_SESSION['obras2']['obrid'] ? $_SESSION['obras2']['obrid'] : $obridVinculado);

    if (!$obrid || !$esfera)
        return false;

    $sql = "
            SELECT count(*)
              FROM obras2.vm_pendencia_obras o
              WHERE o.empesfera = '$esfera' AND o.obrid = {$obrid}
              GROUP BY o.obrid
         ";

    $qtd = $db->pegaUm($sql);
    return $qtd > 0 ? true : false;
}

function removeAnexo()
{
    global $db;

    $obra = new Obras();
    $obra->excluirImagem($_POST['obrid'], $_POST['arqid']);
    $db->commit();
    echo "true";

    die;
}

function salvar()
{

    global $db;

    if (!$_SESSION['obras2']['empid'] || $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $empreendimento = new Empreendimento($_SESSION['obras2']['empid']);

        $endid = $empreendimento->endid;
        // NÃ£o permite ATUALIZAÃÃO de endereÃ§o, sÃ³ INSERÃÃO
        if (!$empreendimento->endid) {
            $endereco = new Endereco($empreendimento->endid);
            $dadosEnd = $_POST['endereco'];
            $dadosEnd['tpeid'] = TIPO_ENDERECO_OBJETO;
            $dadosEnd['endcep'] = str_replace(Array('.', '-'), '', $_POST['endereco']['endcep']);
            $dadosEnd['medlatitude'] = $_POST['graulatitude'] . "." . $_POST['minlatitude'] . "." . $_POST['seglatitude'] . ".S";
            $dadosEnd['medlongitude'] = $_POST['graulongitude'] . "." . $_POST['minlongitude'] . "." . $_POST['seglongitude'] . ".W";
            $endid = $endereco->popularDadosObjeto($dadosEnd)
                ->salvar();
        }

//

        $dados = $_POST['empreendimento'];
        $dados['prfid'] = ($dados['prfid'] ? $dados['prfid'] : null);
        $dados['orgid'] = $_SESSION['obras2']['orgid'];
        $dados['tpoid'] = $_POST['obra']['tpoid'];
        $dados['tobid'] = $_POST['obra']['tobid'];
        $dados['cloid'] = $_POST['obra']['cloid'];
        $dados['endid'] = $_POST['obra']['endid'];
        $dados['empdsc'] = $_POST['obra']['obrnome'];
        $dados['empcomposicao'] = $_POST['obra']['obrcomposicao'];
        $dados['empjustsitdominial'] = $_POST['obra']['obrjustsitdominial'];
        $dados['empvalorprevisto'] = ($_POST['obra']['obrvalorprevisto'] ? MoedaToBd($_POST['obra']['obrvalorprevisto']) : NULL);
        $dados['usucpf'] = $_SESSION['usucpf'];
        $dados['entidunidade'] = $dados['entid'];
        $dados['endid'] = $endid;
        $empid = $empreendimento->popularDadosObjeto($dados)
            ->salvar();

        $_POST['obra']['empid'] = $empid;
    }

    $obrid = $_SESSION['obras2']['obrid'];
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    $arquivo = $_FILES["UploadPhoto"];
    $arqid = "";
    if ($_FILES["UploadPhoto"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
        if ($obrid && $_REQUEST['arqid']) {
            $obra->excluirImagem($obrid, $_REQUEST['arqid']);
        }

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file = new FilesSimec();
        $file->setPasta('obras2');
        $file->setUpload(null, null, false);
        $arqid = $file->getIdArquivo();
    }

    $endid = $obra->endid;
    // NÃ£o permite ATUALIZAÃÃO de endereÃ§o, sÃ³ INSERÃÃO
    if (!$obra->endid) {
        $endereco = new Endereco($obra->endid);
        $dadosEnd = $_POST['endereco'];
        $dadosEnd['tpeid'] = TIPO_ENDERECO_OBJETO;
        $dadosEnd['endcep'] = str_replace(Array('.', '-'), '', $_POST['endereco']['endcep']);
        $dadosEnd['medlatitude'] = $_POST['graulatitude'] . "." . $_POST['minlatitude'] . "." . $_POST['seglatitude'] . ".S";
        $dadosEnd['medlongitude'] = $_POST['graulongitude'] . "." . $_POST['minlongitude'] . "." . $_POST['seglongitude'] . ".W";
        $endid = $endereco->popularDadosObjeto($dadosEnd)
            ->salvar();
    }

    $dados = $_POST['obra'];

    if ($dados['obridvinculado']) {
        $obraVinculada = new Obras();
        $obraVinculada->carregarPorIdCache($dados['obridvinculado']);
        $dados['preid'] = ($obraVinculada->preid ? $obraVinculada->preid : null);
        $dados['obrid_1'] = ($obraVinculada->obrid_1 ? $obraVinculada->obrid_1 : null);
        $dados['tooid'] = $obraVinculada->tooid;
        $dados['obrnumprocessoconv'] = $obraVinculada->obrnumprocessoconv;
        $dados['obranoconvenio'] = $obraVinculada->obranoconvenio;
        $dados['numconvenio'] = $obraVinculada->numconvenio;
    }

    if (empty($obrid)) {
        $dados['obrdtinclusao'] = 'now()';
        $dados['usucpfinclusao'] = $_SESSION['usucpf'];
    }

    $dados['endid'] = $endid;
    $dados['obrvalorprevisto'] = ($dados['obrvalorprevisto'] ? MoedaToBd($dados['obrvalorprevisto']) : NULL);
    $dados['obrperccontratoanterior'] = ($dados['obridvinculado'] ? MoedaToBd($dados['obrperccontratoanterior']) : NULL);
    $dados['usucpf'] = $_SESSION['usucpf'];
    $dados['arqid'] = ($arqid ? $arqid : $dados['arqid']);
    $dados['obridvinculado'] = ($dados['obridvinculado'] ? $dados['obridvinculado'] : NULL);



    $obrid = $obra->popularDadosObjeto($dados)
        ->salvar();

    if ($dados['obridvinculado']) {
        $docid = pegaDocidObra($obrid);
        $obraVinculada->obrstatus = 'P';
        $obraVinculada->salvar();

        if ($obraVinculada->preid) {
            $sql = "UPDATE obras.preobra SET obrid = {$obrid} WHERE preid = {$obraVinculada->preid} AND prestatus = 'A'";
            $db->executar($sql);
        }
        $db->executar("UPDATE workflow.documento SET esdid = " . ESDID_OBJ_LICITACAO . " WHERE docid = $docid");

        wf_alterarEstado($docid, AEDID_OBJ_LICITACAO_LICITACAO, '', array());

        $idObraNova = $obrid;
        $idObraOriginal = $dados['obridvinculado'];

        migraDadosObraVinculada($idObraOriginal, $idObraNova);

    } else {
        $docid = pegaDocidObra($obrid);
    }

    if (is_array($_POST['tpcid'])) {
        foreach ($_POST['tpcid'] as $entid => $id) {
            unset($contatosObra);
            $contatosObra = new ContatosObra();
            $cont['entid'] = $entid;
            $cont['tpcid'] = $id;
            $cont['obrid'] = $obrid;
            $contatosObra->popularDadosObjeto($cont)
                ->salvar();
            $db->commit();
        }
    }

    $db->commit();

    //Caso seja o cadastro de uma nova obra vinculada a uma obra
    if ($dados['obridvinculado']) {
        $idObraNova = $obrid;
        $idObraOriginal = $dados['obridvinculado'];
        posAcaoCadastroObraVinculada($idObraNova, $idObraOriginal);
        $obrid = $idObraOriginal;
    }

    $_SESSION['obras2']['obrid'] = $obrid;
    die('<script>
                alert(\'Operação realizada com sucesso!\');
                location.href=\'?modulo=principal/cadObra&acao=A&obrid=' . $obrid . '\';
         </script>');

}

function litaPrograma()
{

    global $db;

    $programaFonte = new ProgramaFonte();
    $sql = $programaFonte->listaCombo(Array('tpoid' => $_REQUEST['tpoid']));
    if ($sql[0]['codigo'] == '') {
        $db->monta_combo('empreendimento[prfid]', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'prfid', '', $obra->tpoid, '');
    } else {
        $db->monta_combo('empreendimento[prfid]', $sql, 'S', 'Selecione...', '', '', '', 200, 'S', 'prfid', '', $obra->tpoid, '');
    }
}
