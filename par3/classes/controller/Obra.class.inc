<?php
/**
 * Classe de controle do  obra_tipo
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */

/**
 * Par3_Controller_Obra
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_Obra
{
    public $model;
    public $modelHistorico;

    public function __construct($inuid = null, $obrid = null)
    {
        $this->model = new Par3_Model_Obra();
        $this->model->carregarPorId($obrid);
        $this->modelHistorico = new Par3_Model_ObraHistorico();

    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $this->model->popularDadosObjeto();
        $this->model->obrstatus = 'A';
        $this->model->obrcep        = str_replace(array('.', '-'), '', $this->model->obrcep);
        $this->model->obrlatitude   = substr($this->model->obrlatitude, 0, 11);
        $this->model->obrlongitude  = substr($this->model->obrlongitude, 0, 11);
        //$this->model->obrnumero = preg_replace("/[^0-9]/", "", $this->model->obrnumero);
        $this->model->obrnumero = $this->model->obrnumero;

        $inuid = (int)$_GET['inuid'];
        $obrid = (int)$_GET['obrid'];
        $inpid = (int)$_GET['inpid'];
        if(isset($_REQUEST['toReturn'])){
            $url .= "&menu=dados_terreno&inuid={$inuid}&inpid=$inpid&obrid={$obrid}&toReturn={$_REQUEST['toReturn']}";
        }else{
            $url .= "&menu=dados_terreno&inuid={$inuid}&inpid=$inpid&obrid={$obrid}";
        }


        $mAnaliseObra = new Par3_Model_AnaliseEngenhariaObra();
        $analise = $mAnaliseObra->validarAnaliseDadosTerreno($_POST['obrid'],$_POST);
        if(!$analise){
            simec_redirecionar($url, 'error','Corrija os itens da Análise');
        }

        $acao = Par3_Model_Obra::CREATE;
        if ($_POST['obrid'])
            $acao = Par3_Model_Obra::UPDATE;
        try {
            $id = $this->model->salvar();
            $this->model->commit();

            $model = new Par3_Model_Obra($id);
            $model->obrcep          = str_replace(array('.', '-'), '', $model->obrcep);
            $model->obrlatitude     = substr($model->obrlatitude, 0, 11);
            $model->obrlongitude    = substr($model->obrlongitude, 0, 11);
            $model->obrnumero = preg_replace("/[^0-9]/", "", $model->obrnumero);


            $this->modelHistorico->gravarHistorico($model, $acao);

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if ($id) {

            $dados = array('obrid'=> $id,'inuid'=>$_POST['inuid']);


            $obraPlanilhaOrcamento = new Par3_Controller_ObraPlanilhaOrcamentaria();
            $obrvalor = $obraPlanilhaOrcamento->valorTotalPlanilha($dados);

            $obrvalor = $obrvalor ?$obrvalor :0;
            $this->atualizaValorObra($obrvalor,$id,$inpid);

            simec_redirecionar($url, 'success');



        }
        simec_redirecionar($url, 'error');
    }

    public function listar($arrPost = array())
    {
        global $disabled;

   //     $arrQuery = $this->model->getOrdenacaoLista($arrPost);

        $sql = $this->model->montarSQLSimples($arrPost);

        $disabled = 'disabled';

        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Obra ID', 'Tipo de Obra', 'Nome da Obra', 'Valor', 'Situação');

        
        $esconderColunas = array('situacaoworkflow');
        if( $_REQUEST['aba'] == 'analisarPlanejamento' ) $esconderColunas[] = 'situacao';
//ver($esconderColunas);
//        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {
//            $esconderColunas = array('id');array_unshift($cabecalho, 'QTD');
//        }
//        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
//
//        if ($arrPost['requisicao'] == 'imprimir') {
//           $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
//           $disabled = true;
//        }//desabilitar opções para a impressão

        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setId('listTipoObra');
        $listagem->turnOnPesquisator();

        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);


        $listagem->addCallbackDeCampo('obrvalor','par3_mascaraMoeda');

        if( $_REQUEST['aba'] != 'analisarPlanejamento' ){
	        $listagem->addAcao('edit','editarObra');
	        $listagem->addAcao('remove','removerObra');
        }

        $listagem->setAcaoComoCondicional(
            'remove', [['campo' => 'situacaoworkflow', 'op' => 'igual', 'valor' => 'f']]
        );


      //  $listagem->turnOnOrdenacao();
        $listagem->setCabecalho($cabecalho);
        $listagem->render($tratamentoListaVazia);
    }

    public function remover($arrPost)
    {
        $array['obrid'] = $arrPost['obrid'];
        $array['obrstatus'] = 'I';
    	if( ! $this->model->validaExclusaoObraPlanejamento($arrPost['obrid']) )
        {
        	return 'erro_situacao';
        }
        
        try {
            $this->model->popularDadosObjeto($array);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelObra = new Par3_Model_Obra($arrPost['obrid']);

            $this->modelHistorico->gravarHistorico($modelObra, Par3_Model_Obra::DELETE);

            /** função do banco atualiza o valor total na tabela de iniciativa  */
            $sql = "SELECT par3.atualiza_valor_planejado_planejamento_iniciativa(".$arrPost['inpid'].");";
            $result = $this->model->executar($sql);
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    /*public function getComboTipoObra()
    {
        $this->obraTipo = new Par3_Model_ObraTipo();
        $arrFiltro = array("otpstatus = 'A'", "otpsituacao = 'A'");
        if($_REQUEST['etaid']){
            $arrFiltro[] = "etaid = {$_REQUEST['etaid']}";
            $innerEtapa = "INNER JOIN par3.obra_tipo_ensino_etapa oet on oet.otpid = otp.otpid ";
        }
        if($_REQUEST['octid']){
            $arrFiltro[] = "octid = {$_REQUEST['octid']}";
        }
        if($_REQUEST['otpsituacao']){
            $arrFiltro[] = "otpsituacao = '{$_REQUEST['otpsituacao']}'";
        }
        if(count($_REQUEST['NOTIN'])>0){
            foreach($_REQUEST['NOTIN'] as $campo => $valor){
                $arrFiltro[] = "$campo NOT IN ($valor)";
            }
        }
        $sql = "SELECT
                    otp.otpid as codigo,
                    otp.otpdsc as descricao
                FROM par3.obra_tipo otp
                $innerEtapa
                WHERE
                    ".implode(' AND ', $arrFiltro);
        $dados = $this->model->carregar($sql);
        $dados = is_array($dados) ? $dados : array();
//         $dados  = $this->obraTipo->recuperarTodosFormatoInput('otpdsc', $arrFiltro);
        $arr    = simec_preparar_array($dados);
        return $arr;
    }*/

    public function getComboTipoObra()
    {

        $this->inpid = new Par3_Model_IniciativaPlanejamento($inpid);
        $this->obraTipo = new Par3_Model_ObraTipo();
        $dados = $this->obraTipo->getTipoObraPoretapa();
        $qtdLinhas = $this->obraTipo->getTipoObraPoretapa(true);
        $arr = array();

        if(count($qtdLinhas) > 1){
            foreach ($dados as $valor) {
                $arr[$valor['codigo']] = $valor['codigo'].' - '.$valor['descricao'];
            }
        }

        return $arr;
    }

    /**
     * monta o campo metragem para a obra
     * Regras: Caso a obra seja de projeto p?oprio não busca a metragem da versão do projeto
     *
     * @param null $ovpid
     * @param null $disabled
     * @return bool|string
     */
    public function campoMetragem($ovpid = null,$disabled = null)
    {
        $obrmetragem = $this->model->obrmetragem;
        $readOnly = null;
        if($this->model->otpid){
            $otp = new Par3_Model_ObraTipo($this->model->otpid);
            if($ovpid && $otp->otptipoprojeto != 'PP'){
                $mVersaoProjeto = new Par3_Model_ObraTipoVersaoProjeto($ovpid);
                $obrmetragem =  $mVersaoProjeto->ovpmetragem;
                $readOnly = 'readonly="true"';
            }
        }
        return <<<HTML
                    <div class="form-group " id="div_metragem">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Metragem da Obra: </label>
                        <div class="col-sm-8">
                            <div class="input-group col-sm-6">
                                <input id="obrmetragem" class="form-control"  name="obrmetragem" placeholder="Informe a metragem da obra" $readOnly $disabled value="$obrmetragem" >
                                <span class="input-group-addon"><b>M²</b></span>
                            </div>
                        </div>
                    </div>
                    <script>
                        $(function(){
                            $("#obrmetragem").inputmask('numeric');
                        });
                    </script>
HTML;
    }


    public function getComboVersaoProjeto($dados)
    {
        $this->obraTipoVersaoProjeto = new Par3_Model_ObraTipoVersaoProjeto();
        $dados  = $this->obraTipoVersaoProjeto->recuperarTodosFormatoInput('ovpdsc', array("ovpstatus = 'A'", "otpid = {$dados['otpid']}"));
        $arr    = simec_preparar_array($dados);
        return $arr;
    }

    public function getComboVersaoPlanilha($dados)
    {
        $this->obraTipoVersaoPlanilha = new Par3_Model_ObraTipoVersaoPlanilha();
        $dados  = $this->obraTipoVersaoPlanilha->recuperarTodosFormatoInput('tpldsc', array("tplstatus = 'A'", "ovpid = {$dados['ovpid']}"));
        $arr    = simec_preparar_array($dados);
        return $arr;
    }

    public function getDescricaoResumidaObra($obrid, $inuid, $octid)
    {
        $muncod = null;
        $uf = null;

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        if ($instrumentoUnidade->itrid == 2) {
            $muncod = $instrumentoUnidade->muncod;
        } elseif ($instrumentoUnidade->itrid == 1) {
            $uf = $instrumentoUnidade->estuf;
        }
        $sql = $this->model->getDescricaoResumidaObra($obrid, $uf, $muncod, $octid);
//ver($sql);
        $cabecalho = array('R', 'I', 'ID', 'ID Pré-Obra', 'Nº Convênio / Ano', 'Obra', 'Situação da Obra', '% Executado Instituição Acumulado', 'Tipologia');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho)
            ->setQuery($sql)
            ->setId('listarObraTable')
            ->esconderColunas(['estuf', 'muncod'])
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }



    public function criaAbaObra($ano, $inpid, $inuid)
    {// cria aba no planejamento item composição
        $array = array('ano' => $ano, 'inpid' => $inpid, 'inuid' => $inuid);
        global $db;

        echo "<div class=\"ibox float-e-margins\" id=\"itens-ano-$ano\">";
        echo "<div class=\"ibox-title\" id=\"btn-itens-ano-$ano\">";

        echo "<div class=\"ibox-tools\">";
        echo "<a>Opções Avançadas <i id=\"i-itens-ano-$ano\" class=\"fa fa-chevron-down\"></i></a>";
        echo "</div>";
        echo '<h3 class=\"\">' . $ano . '</h3>';
        echo "</div>";

        echo "<div class=\"ibox-content\" id=\"div-itens-ano-$ano\" style=\"display: none;\">";
        echo "<div>";
        if ($ano >= date('Y') && $_REQUEST['aba'] != 'analisarPlanejamento' ) {
            echo "<a href=\"par3.php?modulo=principal/planoTrabalho/obras&acao=A&inuid=$inuid&inpid=$inpid&obrano=$ano\" style=\"width: 110px;\"> <b>+ Incluir Obra </b></a>";
        }
        echo "</div>";

        echo "<div>" . $this->listar($array) . "</div>";
        echo "</div>";
        echo "</div>";
        echo " <script src=\"/library/jquery/jquery.mask.min.js\" type=\"text/javascript\"></script>";
        echo "<script>
                //mostra/esconde ibox
                jQuery(\"#btn-itens-ano-$ano\").click(function () {
                    $('#div-itens-ano-$ano').slideToggle();
                    //chevron up/down
                    $('#i-itens-ano-$ano').toggleClass(function () {
                        if ($('#i-itens-ano-$ano').is(\".fa-chevron-down\")) {
                            $('#i-itens-ano-$ano').removeClass('fa-chevron-down');
                            return 'fa-chevron-up';
                        } else {
                            $('#i-itens-ano-$ano').removeClass('fa-chevron-up');
                            return 'fa-chevron-down';
                        }
                    });
                });


               function editarObra(id) {

                        window.location.assign(\"par3.php?modulo=principal/planoTrabalho/obras&acao=A&inuid=$inuid&inpid=$inpid&obrano=$ano&obrid=\"+id);

                }

             $('.obrvalor_estimado').mask('000.000.000.000.000,00', {reverse: true});


            </script>";
    }
    public function salvarValorEstimado($obrid,$obrvalor){

        global $db;

        $sql = "UPDATE par3.obra SET obrvalor_estimado = $obrvalor WHERE obrid = $obrid";

        $result = $db->executar($sql);
        $db->commit();

        return $result;


    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com>
     * @return string
     * @desciption Esta class retorna os anos que possuem obras no par3
     */
    public function getSqlAnosExistentesObrasPar(){
        return $this->model->getSqlAnosExistentesObrasPar();
    }

    public function listarHistoricoTramitacao($arrPost)
    {
       return $this->model->listarHistoricoTramitacao($arrPost,$this->model->docid);
    }

    public function listarHistoricoTramitacaoModal($docid)
    {
        return $this->model->listarHistoricoTramitacao(array('requisicao'=> 'print'),$docid);
    }

    /**
     * @param $sql string
     * @desciption método retorna sql para o relatório de obras
     * @return string
     */
    public function getSqlListagemObras($arrPOST = null, $boolEsdidCadastramento = true,$xls = false)
    {
        $sql = $this->model->getSqlListagemObras( $arrPOST, $boolEsdidCadastramento,$xls);
        return $sql;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com>
     * @param $arrPost array
     * @desciption Esta class retorna os anos que possuem obras no par3
     * @resturn string
     */
    public function getSqlListagemObrasByUnidade($arrPOST = null){
        if(isset($arrPOST['inuid']) && $arrPOST['inuid'] != ''){
            return $this->model->getSqlListagemObras($arrPOST);
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com>
     * @param $sql string
     * @return int
     * @desciption "Este método retorna 1 caso exista retorn o do sql, e 0 caso não tenha retorno"
     */
    public function returnCountResult($sql){
        return $this->model->getCountResultSql($sql);
    }

    public function atualizaValorObra($valor,$obrid,$inpid)
    {
        global $db;
        $valorObra = str_replace(",", ".", $valor);

        $sql = "UPDATE par3.obra SET obrvalor = {$valorObra} WHERE obrid = {$obrid};
                SELECT par3.atualiza_valor_planejado_planejamento_iniciativa($inpid);";

        $result = $db->executar($sql);
        $db->commit();

        return $result;

    }


    public function createSQLFiltroObras(array $arrPOST){
       return $this->model->createSQLFiltroObras($arrPOST);
    }

    public function createSQLJoinObras(array $arrayPOST){
        return $this->model->createSQLJoinObras($arrayPOST);
    }

    public function listaObraIsValid($preids = null){
        if($preids != null && $preids != ''){
            return  $this->model->listaObrasIsValid($preids);
        }
    }

    public function verificarPerfilEdicao()
    {
        return $this->model->habilitarEdicaoPorPerfil();
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description < Funçao retorna 1 quando o acesso ao formulario e total, 2 so pode ver a unidade dele e 0 não vê pode ver o formulário>
     * @return int
     */
    public function permissaoAcessoObrasMenuPlanejamento(){
        /**
         * - Lista de Obras - FNDE: Superusuário; Administrador; Perfis internos do FNDE;
         *Perfis que podem utilizar filtro completamente, Estado e município
         */
        $arrayPerfisAcessoTotal = array(
            PAR3_PERFIL_ADMINISTRADOR,
            PAR3_PERFIL_SUPER_USUARIO,
            PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR
        );


        /*
         * - Lista de Obras - Município / Estado: Superusuário; Administrador;
         *   Usuários com acesso a lista de estado tem perfil: Secretário Estadual,
         *   Equipe Técnica; Usuários com acesso a lista de município tem perfil: Prefeito, Dirigente e Equipe Técnica;
         */
        $arrayPerfisAcessoParcial = array(
            PAR3_PERFIL_SECRETARIO_ESTADUAL,
            PAR3_PERFIL_EQUIPE_ESTADUAL,
            PAR3_PERFIL_PREFEITO,
            PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO,
            PAR3_PERFIL_EQUIPE_MUNICIPAL
        );

        $cpf = $_SESSION['usucpf'];
        $modelPerfilUsuario = new Par3_Model_PerfilUsuario();
        //carrega todos os perfis atribuidos ao usuário
        $arrayPerfis = $modelPerfilUsuario->getPerfisUsuarioByCPF($cpf);

        $resultPermissaoAcesso = false;
        //testa se usuario possui perfil com total acesso
        foreach($arrayPerfis as $key => $value){
            if(in_array($value['pflcod'],$arrayPerfisAcessoTotal)){
                return 1;
            }
        }
        //testa se usuario possui perfil com parcial acesso
        foreach($arrayPerfis as $key => $value){

            if(in_array($value['pflcod'],$arrayPerfisAcessoParcial)){
                return 2;
            }
        }

        //testa se usuario mnão possui acesso
        foreach($arrayPerfis as $key => $value){

            if(!in_array($value['pflcod'],$arrayPerfisAcessoTotal) && !in_array($value['pflcod'],$arrayPerfisAcessoParcial)){
                return 0;
            }
        }
    }

    /**
     * Valida se as obras que irão para aprovação possuem o mesmo Estado Documento
     * @param array $obras
     * @return bool
     */
    public function validarAprovacaoObras(array $obras)
    {
        include_once APPRAIZ ."includes/workflow.php";
//        require_once APPRAIZ . '/includes/workflow.php';
        $mObra    = new Par3_Model_Obra($obras[0]);
//        ver($mObra->obrid,$mObra->docid);
        $arrEsd   = wf_pegarEstadoAtual($mObra->docid);
        $docid    = $mObra->docid;
        $arrDocid = array();
        $arrObras = array();
        foreach($obras as $obrid){
            $arrObrDadosTramitacao =  $mObra->recuperarTodos('t1.*,t2.otpdsc,t3.itrid,t3.muncod,t3.inudescricao', array("obrid = {$obrid}"), '1 limit 1', array('join' => array('otpid','inuid')));
            $esdid = wf_pegarEstadoAtual($arrObrDadosTramitacao[0]['docid']);
            if($arrEsd['esdid'] != $esdid['esdid']){
                return 0;
            }
            $arrObras[] = $arrObrDadosTramitacao[0];
            $arrDocid[] = array('obrid' => $arrObrDadosTramitacao[0]['obrid'],'docid' => $arrObrDadosTramitacao[0]['docid']);
        }
        $arrProximosEstados =  wf_pegarProximosEstados($docid);
        require APPRAIZ . 'par3/modulos/principal/aprovacaoLoteObra.inc';
    }

    /**
     * Realiza tramitação em lote para documentos que possuem o mesmo estadodocumento
     * @param array $arrDocid
     * @param $aedid
     * @param string $comentario
     */
    public function tramitarEmLote(array $arrDocid,$aedid,$comentario = '')
    {
        include_once APPRAIZ ."includes/workflow.php";
        $retorno = array();
        foreach ($arrDocid as $arrObr) {
           $retorno[] = wf_alterarEstado($arrObr['docid'],$aedid, $comentario.(' (Tramitado em lote)'), array('obrid' => $arrObr['obrid']));
        }
        if(in_array(0,$retorno)){
            return 0;
        }
        return 1;
    }

    public function imprimeObrasXls($arrPOST = null,$boolesdidcadastramento = true)
    {
        $arListagem = $this->getSqlListagemObras($arrPOST,$boolesdidcadastramento,true);
        $_SESSION['toReturn'] = $_POST;
        $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);

        $arrayCabecalho = array(
            'Pré-Obra',
            'Nome',
            'Tipo obra',
            'Esfera',
            'Município',
            'UF',
            'Escola',
            'Situação',
            'Data de Tramitação',
            'Analista',
            'Obras 2.0',
            'Situação Obras 2.0',
            'Processo',
            'Etapa' ,
            'Ano',
            'Contrapartida',
            'Valor MEC/FNDE',
            'Valor Obra',
            'Pagamento',
            'Termo',
            'Empenho'
        );
        $arrayEsconder = array('unidade','iniciativapla','edit','situacao_estado_documento_id', 'obrid','obrcheck','linkeditarobr','docid','linkobra2', 'dotid');
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($arListagem);
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($arrayEsconder);
        $listagemSimec->addCallbackDeCampo('desdobramentos', 'listaDesdobramentosListaObras');
        $listagemSimec->addCallbackDeCampo('obrvalor','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('obrvalor_contrapartida','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('Empenho','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('valor_fnde','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('vlrpago','formataNumeroMonetarioSemSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS,'preid');
        $listagemSimec->addCallbackDeCampo('Processo','formata_numero_processo');
        $listagemSimec->setId('xlsobras');
        $_REQUEST['filtroObras'] = $_POST;
        return $listagemSimec->render();
    }

    public function gerarExtratoObra()
    {
        $mOed = new Par3_Model_ObraEstudoDemanda();
        $arrOed = $mOed->recuperarPorObra($this->model->obrid);
        $mItr = new Par3_Controller_InstrumentoUnidade();

        if($_GET['inpid'] == null){
            $mObra = new Par3_Model_Obra($_GET['obrid']);
            $mInp  = new Par3_Controller_IniciativaPlanejamento($mObra->inpid);
            $inp   = new Par3_Model_IniciativaPlanejamento($mObra->inpid);
        }else{
            $mInp = new Par3_Controller_IniciativaPlanejamento($_GET['inpid']);
            $inp  = new Par3_Model_IniciativaPlanejamento($_GET['inpid']);
        }
        $oct = $inp->recuperarTipoObra();

        //Recupera a dimensão
        $arrItrid['itrid'] = $mItr->pegarItrid($_GET['inuid']);
        $mDim = new Par3_Model_Dimensao();
        $arrDim = $mDim->listarSelect($arrItrid);

        //Recupera iniciativa disponível para o instrumento unidade
        $arrIniciativa = $mInp->getIniciativa(['inuid' => $_GET['inuid'], 'iniid' => $inp->iniid]);
        //Recupera os dados da iniciativa selecionada
        $arrIniciativaDados = $mInp->getIniciativaDados(array('iniid' => $inp->iniid));
        //Recupera as modalidades de acordo com a etapa selecionada
        $arrMod = $mInp->getIniciativaModalidades(array('iniid' => $inp->iniid, 'etaid' => $inp->etaid), true);
        //recupera os desdobramentos de acordo com a iniciativa,nível e etapa selecionados
        $arrDes = $mInp->getIniciativaDesdobramentos(array('iniid' => $inp->iniid, 'etaid' => $inp->etaid, 'modid' => $inp->modid), true);
        //busca Etapa
        $mEta = new Par3_Model_EnsinoEtapa($inp->etaid);
        //Recupera  Desdobramentos selecionado
        $desdobramentos = $mInp->getDesdobramentosSelecionado($inp->inpid);

        //DADOS ESTUDO DE DEMANDA
        $arrOed = $this->model->carregarDadosExtratoObraEstudoDemanda($this->model->obrid);
        $mInu   = new Par3_Model_InstrumentoUnidade($_GET['inuid']);
        $mOvp   = new Par3_Model_ObraTipoVersaoProjeto($this->model->ovpid);

        //RETORNAR METRAGEM DA OBRA
        $obrmetragem = $this->model->obrmetragem;
        if($this->model->otpid){
            $otp = new Par3_Model_ObraTipo($this->model->otpid);
            if($this->model->ovpid && $otp->otptipoprojeto != 'PP'){
                $mVersaoProjeto = new Par3_Model_ObraTipoVersaoProjeto($mOvp->ovpid);
                $obrmetragem =  $mVersaoProjeto->ovpmetragem;
            }
        }
        //RECUPERAR ID OBRAS 2.0 SE EXISTIR
        $mObra2 = $this->model->recuperar("select * from obras2.obras obra2 where obra2.obrid_par3 = {$this->model->obrid}");

        $mProcesso = $this->model->recuperar("select * from obras2.obras obra2 where obra2.obrid_par3 = {$this->model->obrid}");

        //Recuperar processo,termo e empenho
        $arrDados = $this->model->recuperaEmpenhoTermoProcessoPorObra($this->model->obrid);
        $processo = $arrDados['processo']?$arrDados['processo']:'';
        $termo    = $arrDados['termo']?$arrDados['termo']:'';
        $mpenho   = $arrDados['empenho']?$arrDados['empenho']:'';
//        extract($arrDados);//$processo,$termo,$empenho
//        ver($numeroProcesso,d);

        $mtp    = new Par3_Model_ObraTipo($this->model->otpid);
        $obrXQ  = new Par3_Model_ObraXQuestionario();
        $rsObrq = $obrXQ->recuperarTodos('t2.escnome,t1.*',array("obrid = {$this->model->obrid}"),null,array('join'=>'escid'));
        $mDoc   = new Par3_Controller_ObraFotosTerreno(0);
        $arDoc  = $this->model->carregar($mDoc->getSqlLista(['obrid' => $this->model->obrid]));
        $mAnexo = new Par3_Model_ObraDocumentos();
        $arrAnexo = $mAnexo->carregar($mAnexo->montarSQLSimples(['obrid' => $this->model->obrid]));

        require(APPRAIZ . 'par3/modulos/principal/planoTrabalho/obras/extrato_obra/extrato_obra.inc');
    }


    public function detalharHistoricoAnalise($aegid,$obrid)
    {
        $sqlHistorico = $this->model->obterAnalisePorHistorico($aegid,$obrid);
        $historicoAnalise = $this->model->carregar($sqlHistorico);
        return $historicoAnalise;
//        $sqlComentario = $this->model->obterComentariosHistoricoAnalise($arrParans);
//        $comentarios = $this->model->carregar($sqlComentario);

//        include APPRAIZ.'par3/modulos/principal/planoTrabalho/obras/detalhar_historico_tramitacao.inc';
    }
}
?>