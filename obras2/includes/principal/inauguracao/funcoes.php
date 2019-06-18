<?php


function verificaArqidInauguracao($iobid, $arqid)
{
    global $db;
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivo_descricao_inauguracao'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }

    $arqid = (is_array($arqid)) ? $arqid : array();
    $sql = "
        SELECT fio.arqid
        FROM obras2.fotos_inauguracao_obra fio
        JOIN arquivo a ON a.arqid = fio.arqid
        WHERE fio.fstatus = 'A' AND fio.iobid = $iobid AND fio.tipo = 'I'
    ";
    $arquivos  = $db->carregar($sql);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $sql = "UPDATE obras2.fotos_inauguracao_obra SET fstatus = 'I' WHERE arqid = {$arquivo['arqid']}";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
}

function verificaArqidObraConcluida($iobid, $arqid)
{
    global $db;
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivo_descricao_obra_concluida'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }

    $arqid = (is_array($arqid)) ? $arqid : array();
    $sql = "
        SELECT fio.arqid
        FROM obras2.fotos_inauguracao_obra fio
        JOIN arquivo a ON a.arqid = fio.arqid
        WHERE fio.fstatus = 'A' AND fio.iobid = $iobid AND fio.tipo = 'C'
    ";
    $arquivos  = $db->carregar($sql);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $sql = "UPDATE obras2.fotos_inauguracao_obra SET fstatus = 'I' WHERE arqid = {$arquivo['arqid']}";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
}

function importaDadosInauguracao($obrid)
{
    global $db;
    $sql = <<<DML
SELECT iobid,
       capacidade,
       investimentototal,
       dtiniciofuncionamento,
       dtprevisaoinauguracao,
       alunosmatriculados,
       statuspc,
       recursos_repassados,
       mobiliario_adquirido,
       mobiliario_entregue,
       quantidadehabitantes,
       distancia,
       aeroportos,
       outras,
       justificativaprevisao,
       empreendimento_apto
  FROM obras2.inauguracao_obra
    WHERE obrid = {$obrid}
      AND iobstatus = 'A'
DML;
    $dados = $db->pegaLinha($sql);
    return $dados;
}

function importaFotosDadosInauguracao($iobid) {
    global $db;
    if (!$iobid) {
        return array();
    }
    $sql = <<<DML
SELECT fio.tipo,
       fio.iobid,
       fio.arqid,
       arq.arqnome,
       arq.arqdescricao,
       arq.arqextensao,
       fio.fdtinclusao
  FROM obras2.fotos_inauguracao_obra fio
    INNER join public.arquivo arq ON fio.arqid = arq.arqid
    WHERE fio.iobid = {$iobid}
      AND fio.fstatus = 'A'
      AND arq.arqstatus = 'A'
DML;
    $dados = $db->carregar($sql);
    return $dados;
}

function importaDadosHabiteSe($iobid = null) {
    global $db;
    if ( ! $iobid) {
        return false;
    }
    $sql = 
	"select 
		'<img src=../imagens/icone_lupa.png title=\"Visualizar\" style=cursor:pointer; onclick=\"downloadArquivo('|| aht.arqid ||');\">' as acao,
		formata_cpf_cnpj(usu.usucpf ) || ' - ' || usunome as responsavel,
		TO_CHAR(ahtdatainclusao , 'DD/MM/YYYY - HH24:MI:SS') as data_inclusao
	 from obras2.anexohabitese aht
	 INNER JOIN seguranca.usuario usu ON usu.usucpf = aht.usucpf
	 where iobid = {$iobid}
    order by ahtdatainclusao desc
";
    $dados = $db->carregar($sql);
    return $dados;
}

function salvarDados($dados) {
    global $db;
    $iobid = $dados['iobid'];
    $capacidade = $dados['capacidade'] ? limparNumeros($dados['capacidade']) : 'NULL';
    $investimentototal = $dados['investimentototal'] ? MoedaToBd($dados['investimentototal']) : 'NULL';
    $dtiniciofuncionamento = $dados['dtiniciofuncionamento'] ? "'".$dados['dtiniciofuncionamento']."'" : 'NULL';
    $dtprevisaoinauguracao = $dados['dtprevisaoinauguracao'] ? "'".$dados['dtprevisaoinauguracao']."'" : 'NULL';
    $empreendimentoApto = $dados['empreendimento_apto'] ? "'".$dados['empreendimento_apto']."'" : 'NULL';
    $alunosmatriculados = $dados['alunosmatriculados'] ? limparNumeros($dados['alunosmatriculados']) : 'NULL';
    $statuspc = $dados['statuspc'];
    $recursos_repassados = ($dados['recursos_repassados']) ? "'" . $dados['recursos_repassados'] . "'": 'null';
    $mobiliario_adquirido = ($dados['mobiliario_adquirido']) ? "'" . $dados['mobiliario_adquirido'] . "'" : 'null';
    $mobiliario_entregue = ($dados['mobiliario_entregue']) ? "'" . $dados['mobiliario_entregue'] . "'": 'null';
    $quantidadehabitantes = $dados['quantidadehabitantes'] ? limparNumeros($dados['quantidadehabitantes']) : 'NULL';
    $distancia = $dados['distancia'] ? limparNumeros($dados['distancia'])  : 'NULL';
    $aeroportos = $dados['aeroportos'];
    $outras = $dados['outras'];
    $obrid = $dados['obrid'];
    $usucpf = $_SESSION['usucpf'];
    $justificativaprevisao = filter_var($dados['justificativaprevisao'], FILTER_SANITIZE_FULL_SPECIAL_CHARS);

    try {
        if ($iobid) {
            $sql = <<<DML
                UPDATE obras2.inauguracao_obra
                    SET capacidade = $capacidade, investimentototal = $investimentototal, dtiniciofuncionamento = $dtiniciofuncionamento,
                        dtprevisaoinauguracao = $dtprevisaoinauguracao, alunosmatriculados = $alunosmatriculados,
                        statuspc = '$statuspc',
                        recursos_repassados = $recursos_repassados,
                        mobiliario_adquirido = $mobiliario_adquirido,
                        mobiliario_entregue = $mobiliario_entregue,
                        empreendimento_apto = $empreendimentoApto,

                        quantidadehabitantes = $quantidadehabitantes, distancia = $distancia,
                        aeroportos = '$aeroportos', outras = '$outras', justificativaprevisao = '{$justificativaprevisao}'
                WHERE obrid = $obrid
                    AND iobstatus = 'A'
                    AND iobid = {$iobid};
DML;
            $db->executar($sql);
            $db->commit();
        } else {
            $sql = <<<DML
                INSERT INTO obras2.inauguracao_obra
                    (capacidade, investimentototal, dtiniciofuncionamento, dtprevisaoinauguracao, alunosmatriculados,
                    statuspc, recursos_repassados, mobiliario_adquirido, mobiliario_entregue, quantidadehabitantes, distancia, aeroportos, outras, obrid, usucpf, justificativaprevisao
                    ,empreendimento_apto)
                VALUES ($capacidade, $investimentototal, $dtiniciofuncionamento, $dtprevisaoinauguracao,
                    $alunosmatriculados, '$statuspc',$recursos_repassados ,$mobiliario_adquirido ,$mobiliario_entregue , $quantidadehabitantes, $distancia, '$aeroportos', '$outras',
                    '$obrid', '$usucpf', '{$justificativaprevisao}',
                    $empreendimentoApto) RETURNING iobid;
DML;
                    

            $iobid = $db->pegaUm($sql);
            $db->commit();
        }

        if (!empty($justificativaprevisao)) {
            $data = (new DateTime('now'))->format('d/m/Y');
            $justificativaprevisao = <<<TEXTO
Na data {$data} o usuário {$_SESSION['usunome']} informou sob a justificativa "{$justificativaprevisao}" o motivo do funcionamento da obra ultrapassar o período de 1 ano a data de conclusão informado na vistoria.
TEXTO;

            $regAtividade = new RegistroAtividade();
            $regAtividade->popularDadosObjeto([
                'obrid' => $obrid,
                'usucpf' => $usucpf,
                'rgadscsimplificada' => 'Inserida justificativa para o funcionamento da obra após 1 da conclusão',
                'rgadsccompleta' => $justificativaprevisao,
            ]);
            $regAtividade->salvar();
            $regAtividade->commit();
        }
		
        if ($_FILES['arquivo_obra_habite_se']['name'] != '') {
        	
        	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        	$arquivos = $_FILES;
        	
        	$campos = array(
                "iobid" => $iobid,
                "ahtdatainclusao" => "NOW()",
                "usucpf" => "'{$_SESSION['usucpf']}'"
            );
              
            $file = new FilesSimec("anexohabitese", $campos ,"obras2");
            $file->setUpload( $_FILES['arquivo_obra_habite_se']['name'] , null, true, 'ahtid' );
            $ahtid = $file->getCampoRetorno();
            $db->commit();
            
        }
        
        
        $arrArqid = $dados['arquivo_obra_concluida'] ? $dados['arquivo_obra_concluida'] : array();
        if ($_FILES['arquivo_obra_concluida']['name'][0] != '') {
            require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $arquivos = $_FILES;
            foreach ($arquivos['arquivo_obra_concluida']['name'] as $key => $value) {
                if(empty($value))
                    continue;
                
                if (in_array($arquivos['arquivo_obra_concluida']['type'][$key], array('images/png', 'images/jpg', 'images/jpeg', 'images/bmp', 'images/gif'))) {
                    echo "<script> alert('Somente imagens são permitidas em Obra Funcionamento!'); </script>";
                    return false;
                }
                if ($arquivos['arquivo_obra_concluida']['size'][$key] > 4194304) {
                    echo "<script> alert('O tamanho máximo do arquivo em Obra Funcionamento deve ser de 4 MB!'); </script>";
                    return false;
                }
                $files =  array(
                    'name' => $arquivos['arquivo_obra_concluida']['name'][$key],
                    'type' => $arquivos['arquivo_obra_concluida']['type'][$key],
                    'tmp_name' => $arquivos['arquivo_obra_concluida']['tmp_name'][$key],
                    'error' => $arquivos['arquivo_obra_concluida']['error'][$key],
                    'size' => $arquivos['arquivo_obra_concluida']['size'][$key]
                );
                $_FILES['arquivo'] = $files;
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload($dados['arquivo_descricao_obra_concluida'][$key], 'arquivo', false);
                $arqid = $file->getIdArquivo();
                if ($arqid) {
                    $arrArqid[] = $arqid;
                    $sql = <<<DML
                        INSERT INTO obras2.fotos_inauguracao_obra (iobid, tipo, arqid, obrid, usucpf) VALUES ($iobid, 'C', $arqid, $obrid, '{$_SESSION['usucpf']}');
DML;
                    $db->executar($sql);
                }
            }
            $db->commit();
        }
        verificaArqidObraConcluida($iobid, $arrArqid);

        $arrArqid = $dados['arquivo_inauguracao'] ? $dados['arquivo_inauguracao'] : array();
        if ($_FILES['arquivo_inauguracao']['name'][0] != '') {
            require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $arquivos = $_FILES;
            foreach ($arquivos['arquivo_inauguracao']['name'] as $key => $value) {
                if(empty($value))
                    continue;
                
                if (in_array($arquivos['arquivo_obra_concluida']['type'][$key], array('images/png', 'images/jpg', 'images/jpeg', 'images/bmp', 'images/gif'))) {
                    echo "<script> alert('Somente imagens são permitidas em fotos de Inauguração!'); </script>";
                    return false;
                }
                if ($arquivos['arquivo_obra_concluida']['size'][$key] > 4194304) {
                    echo "<script> alert('O tamanho máximo do arquivo em fotos de Inauguração deve ser de 4 MB!'); </script>";
                    return false;
                }
                $files = array(
                    'name' => $arquivos['arquivo_inauguracao']['name'][$key],
                    'type' => $arquivos['arquivo_inauguracao']['type'][$key],
                    'tmp_name' => $arquivos['arquivo_inauguracao']['tmp_name'][$key],
                    'error' => $arquivos['arquivo_inauguracao']['error'][$key],
                    'size' => $arquivos['arquivo_inauguracao']['size'][$key]
                );
                $_FILES['arquivo'] = $files;
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload($dados['arquivo_descricao_inauguracao'][$key], 'arquivo', false);
                $arqid = $file->getIdArquivo();

                if ($arqid) {
                    $arrArqid[] = $arqid;
                    $sql = <<<DML
                        INSERT INTO obras2.fotos_inauguracao_obra (iobid, tipo, arqid, obrid, usucpf) VALUES ($iobid, 'I', $arqid, $obrid, '{$_SESSION['usucpf']}');
DML;
                    $db->executar($sql);
                }
            }
            $db->commit();
        }
        verificaArqidInauguracao($iobid, $arrArqid);
        $db->sucesso('principal/inauguracao', '', 'Registro salvo com sucesso!');
    } catch (Exception $ex) {
        
    }
}
