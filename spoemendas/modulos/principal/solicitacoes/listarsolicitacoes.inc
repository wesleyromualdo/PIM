<?php
/**
 * Listagem de emendas para edição.
 *
 * @version $Id: listaremendas.inc 109643 2016-03-31 19:49:39Z werteralmeida $
 */
include_once APPRAIZ . 'includes/workflow.php';
$fm = new Simec_Helper_FlashMessage('solicitacoes');
$solicitacao = new Spoemendas_Model_Solicitacoes();

/* Filtros */
$where = "WHERE true ";
if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao']))
{
  $requisicao = $_REQUEST['requisicao'];
  $dados = $_REQUEST['filtrosolicitacoes'];
  switch ($requisicao)
  {
    case 'filtrosolicitacoes':
      if ($dados['usucpf'] <> '' && $dados['usucpf'] <> 'undefined') {
          $where .= " AND usu.usucpf = '{$dados['usucpf']}'";
      }
      if ($dados['emeid'] <> '' && $dados['emeid'] <> 'undefined') {
          $where .= " AND eme.emeid = '{$dados['emeid']}'";
      }
      if ($dados['autid'] <> '' && $dados['autid'] <> 'undefined') {
          $where .= " AND aut.autid = '{$dados['autid']}'";
      }
      if ($dados['tpsid'] <> '' && $dados['tpsid'] <> 'undefined') {
          $where .= " AND tps.tpsid = '{$dados['tpsid']}'";
      }
      if (is_array($dados['esdid'])) {
          $estados = implode(',', $dados['esdid']);
          $where .= " AND doc.esdid IN ({$estados})";
      } elseif ($dados['esdid'] <> '' && $dados['esdid'] <> 'undefined'){
          $where .= " AND esd.esdid = '{$dados['esdid']}'";
      }
      break;
    case 'drawWorkflow':
      list(,$docid, $ofiid) = $_POST['params'];
      wf_desenhaBarraNavegacaoBootstrap($docid, ['docid' => $docid, 'ofiid' => $ofiid]);
      die();
    case 'apagar_anexo':
      global $db;
      $sql =<<<DML
      DELETE FROM spoemendas.oficiotiposolicitacao where ofiid={$_REQUEST['ofiid']}
DML;
      $db->executar($sql);
      $sql =<<<DML
      DELETE FROM emenda.oficio where arqid={$_REQUEST['arqid']}
DML;
      $db->executar($sql);

      $file = new FilesSimec('oficio', 'arqid' ,'spoemendas');
      $file->setPulaTableEschema(true);
      $file->setRemoveUpload($_REQUEST['arqid']);
    break;
    case 'baixar_anexo':
      (new Spoemendas_Model_Anexogeral())->downloadAnexo($_REQUEST['arqid'], 'emenda', 'oficio');
    break;
  }
}

?>
<style>
    *{
        white-space: normal !important;
    }
</style>
<?php
/**
 * Cabeçalho do SIMEC.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-md-12">
<script type="text/javascript">
function apagarAnexo(arqid, ofiid)
{
  if (confirm('Confirma a exclusão do anexo?')) {
    window.location.assign(
      'spoemendas.php?modulo=principal/solicitacoes/listarsolicitacoes&acao=A&requisicao=apagar_anexo'
      + '&arqid=' + arqid
      + '&ofiid=' + ofiid
    );
  }
}
function baixarAnexo(arqid) {
    window.location.assign(
      'spoemendas.php?modulo=principal/solicitacoes/listarsolicitacoes&acao=A&requisicao=baixar_anexo'
      + '&arqid=' + arqid
    );
}
</script>
<?php
    
    (new Simec_View_Breadcrumb())
      ->add('Solicitações de informação ou alteração')
      ->render();
    
    /* Filtros por Perfil */
    $perfis = pegaPerfilGeral($_SESSION['usucpf']);

    $form = new Simec_View_Form('filtrosolicitacoes');
    $tpdidSolicitacao = WF_TPDID_SPOEMENDAS_SOLICITACOES;
    $tpdidOficio = WF_TPDID_SPOEMENDAS_ANEXO_OFICIO;
    
    /* Filtrando o autor (perfil Parlamentar) */
    $whereAutor = $solicitacao->retornaWhereAutorSolicitacao();
    $listaEstadosDocumento = "SELECT
                                    esdid  AS codigo,
                                    esddsc AS descricao
                                FROM
                                    workflow.estadodocumento where tpdid = {$tpdidSolicitacao}
                                        AND esdstatus = 'A'
                                ORDER BY 2";
    $listaSolicitantes = "SELECT DISTINCT
                                    usucpf  AS codigo,
                                    usunome AS descricao
                                FROM
                                    seguranca.usuario_sistema
                                JOIN
                                    seguranca.usuario
                                USING
                                    (usucpf)
                                WHERE
                                    sisid = 237
                                    AND susstatus = 'A'
                                ORDER BY 2";
    $listaEmendas = "SELECT
                    eme.emeid as codigo,
                    eme.emeano || '/' || eme.emecod AS descricao
                FROM
                    emenda.emenda eme
                JOIN
                    emenda.autor aut
                USING
                    (autid)
                WHERE true {$whereAutor}
                ORDER BY
                    eme.emeano DESC,
                    eme.emecod";
    $listaParlamentares = "SELECT
                            aut.autid AS codigo,
                            aut.autnome || ' (' || par.parsigla || ')' AS descricao
                        FROM
                            emenda.autor aut
                        JOIN
                            emenda.partido par
                        USING
                            (parid)
                        WHERE
                            autid IN
                            (
                                SELECT
                                    autid
                                FROM
                                    emenda.emenda
                                --WHERE emeano = '{$_SESSION['exercicio']}'
                            )
                             {$whereAutor}
                        ORDER BY
                            2";
    $listaStatusAnexo = "SELECT
                          esdid as codigo,
                          esddsc as descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = {$tpdidOficio}
                        ORDER BY esdordem";
    $form->carregarDados($_REQUEST['filtrosolicitacoes'])
      ->addInputCombo('autid', $listaParlamentares, $_REQUEST['filtrosolicitacoes']['autid'], 'emeid', array('flabel' => 'Solicitante'))
      ->addInputCombo('emeid', $listaEmendas, $_REQUEST['filtrosolicitacoes']['emeid'], 'emeid', array('flabel' => 'Emenda'))
      ->addChoices('Tipo de Solicitação', 'tpsid', array('Modalidade' => '1', 'GND' => '2', 'Ação' => '3', 'UO' => '4', 'Demais solicitações' => '5'), array('class-group' => 'tipo_solicitacao'))
      ->addInputCombo('esdid', $listaStatusAnexo, $_REQUEST['filtrosolicitacoes']['esdid'], 'esdid', array('flabel' => 'Status do anexo'))
      ->addBotoes([ 'limpar', 'buscar'])
      ->setRequisicao('filtrosolicitacoes')
      ->render();
    $listagem = new Simec_Listagem();
    
    /* Filtra pelo usuário logado, para UO */
    if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
        $_REQUEST['filtrosolicitacoes']['usucpf'] = $_SESSION['usucpf'];
    }
    /* Filtra pelo usuário logado, para Parlamentar */
    if (in_array(PFL_PARLAMENTAR, $perfis)) {
        $where .= $whereAutor;
    }
    
    $sql = "SELECT
              ofi.arqid,
              doc.docid,
              ofi.ofiid,
              esd.esdid,
              aut.autnome || ' (' || par.parsigla || ')' AS solicitante,
              eme.emeano || '/' || eme.emecod as emendas,
              tps.tpsid AS tiposolicitacaocod,
              tps.tpsdsc AS tiposolicitacaodsc,
              usu.usunome AS anexou,
              TO_CHAR(ofi.oficriacao, 'DD/MM/YYYY HH24:MI:SS') as anexodata,
              esd.esddsc AS status,
              eme.emecod,
              eme.emeid
            FROM emenda.oficio ofi
            JOIN spoemendas.oficiotiposolicitacao oft ON oft.ofiid = ofi.ofiid
            JOIN spoemendas.tiposolicitacao tps ON tps.tpsid = oft.tpsid
            JOIN emenda.autor aut ON ofi.autid = aut.autid
            JOIN emenda.partido par ON aut.parid = par.parid
            JOIN seguranca.usuario usu ON ofi.usucpf = usu.usucpf
            JOIN workflow.documento doc ON ofi.docid = doc.docid
            JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
            JOIN emenda.emenda eme ON aut.autid = eme.autid AND eme.emeid = ofi.emeid
            {$where}
            GROUP BY
            	 ofi.arqid
            	,doc.docid
            	,ofi.ofiid
            	,esd.esdid
            	,solicitante
            	,emendas
            	,tiposolicitacaocod
            	,tiposolicitacaodsc
            	,anexou
            	,status
            	,eme.emecod
            	,eme.emeid
            	";
    echo $fm;
    $listagem->setQuery($sql)
      ->setCabecalho(['Solicitante','Emenda','Tipo solicitação','Quem anexou','Data e Hora','Status do anexo'])
      ->addCallbackDeCampo('emendas', 'formatarEmendasListagemSolicitacao')
      ->esconderColunas('esdid','arqid','ofiid','tpsid','emecod','emeid','docid','tiposolicitacaocod')
      ->addAcao('download', 'baixarAnexo')
      ->addAcao('workflow', array('func' => 'drawWorkflow','extra-params' => array('docid', 'ofiid')))
      ->addAcao('delete', array('func' => 'apagarAnexo','extra-params' => array('ofiid')))
      ->turnOnPesquisator()
      ->setFormFiltros('filtrosolicitacoes')
      ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>
</div>
