<?php
/**
 * Description of Comunicado
 *
 * @author LindalbertoFilho
 */
require_once APPRAIZ .'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

class Comunicado extends Modelo{

    protected $_modulo;
    protected $_objeto;

    public function __construct($modulo) {
        $this->_modulo = $modulo;
        $this->_objeto = $this->_verificaModulo();
    }

    protected function _verificaModulo(){
        switch($this->_modulo){
            case 'planacomorc':
                include_once APPRAIZ . "includes/SpoComunicados/classes/PlanacomorcComunicado.inc";
                return new PlanacomorcComunicado();
            case 'altorc':
                include_once APPRAIZ . "includes/SpoComunicados/classes/AltorcComunicado.inc";
                return new AltorcComunicado();
            case 'progfin':
                include_once APPRAIZ . "includes/SpoComunicados/classes/ProgfinComunicado.inc";
                return new ProgfinComunicado();
            case 'progorc':
                include_once APPRAIZ . "includes/SpoComunicados/classes/ProgorcComunicado.inc";
                return new ProgorcComunicado();
            case 'proporc':
                include_once APPRAIZ . "includes/SpoComunicados/classes/ProporcComunicado.inc";
                return new ProporcComunicado();
            case 'recorc':
                include_once APPRAIZ . "includes/SpoComunicados/classes/RecorcComunicado.inc";
                return new RecorcComunicado();
            case 'sicaj':
                include_once APPRAIZ . "includes/SpoComunicados/classes/SicajComunicado.inc";
                return new SicajComunicado();
            case 'ted':
                include_once APPRAIZ . "includes/SpoComunicados/classes/TedComunicado.inc";
                return new TedComunicado();
            case 'acomporc':
                include_once APPRAIZ . "includes/SpoComunicados/classes/AcomporcComunicado.inc";
                return new AcomporcComunicado();
            case 'siafi':
                include_once APPRAIZ . "includes/SpoComunicados/classes/SiafiComunicado.inc";
                return new SiafiComunicado();
            case 'spoemendas':
                include_once APPRAIZ . "includes/SpoComunicados/classes/SpoEmendasComunicado.inc";
                return new SpoEmendasComunicado();
        }
    }

    public function buscar($id){
        $this->_objeto->__set('arqid',$id);
        return $this->_objeto->buscar();

    }

    public function cadastrar(){
        return $this->_objeto->cadastrar();

    }

    public function atualizar($dados = null){
        $this->_objeto->popularDadosObjeto($dados);
        return $this->_objeto->atualizar();

    }

    public function deletar($id){

        if(in_array(get_class($this->_objeto), ["RecorcComunicado", "SicajComunicado"])){
            $this->_objeto->__set('angid',$id);
        }else{
            $this->_objeto->__set('arqid',$id);
        }

        return $this->_objeto->deletar();
    }

    public function listar(){
        return $this->_objeto->listar();
    }

    public function listaInicial(){
        return $this->_objeto->listaInicial();
    }

    public function buscarDescricao($id){
        $this->_objeto->__set('arqid',$id);
        return $this->_objeto->buscarDescricao();
    }

    public function obterComunicados($usucpf = null){
        if(in_array($this->_modulo , ['recorc', 'sicaj'])){
            return $this->_objeto->obterComunicados($usucpf);
        }
        return false;
    }
    public function listarComunicados($condicao = null){

        if(in_array($this->_modulo , ['recorc', 'sicaj'])){
            return $this->_objeto->listarComunicados($condicao);
        }
        return false;
    }

    public function listarComunicadoEditar($angid){
        if(in_array($this->_modulo , ['recorc', 'sicaj'])){
            return $this->_objeto->listarComunicadoEditar($angid);
        }
        return false;
    }

    public function enviaEmail() {

        /* configura��es */
        ini_set("memory_limit", "2048M");
        set_time_limit(600);
        global $db;

        # captura as informa��es submetidas
        $perfis = (array) $_REQUEST["perfil"];
        $arrPerfis = implode(',', $perfis);
        # identifica os destinat�rios
        $destinatarios = array();
        $arrayDestinatarios = array();

        $sql = "SELECT DISTINCT
                    u.usunome,
                    u.usuemail,
                    u.usucpf,
                    u.regcod
                FROM
                    seguranca.usuario_sistema usi
                JOIN
                    seguranca.perfilusuario pfu
                USING
                    (usucpf)
                JOIN
                    seguranca.usuario u
                USING
                    (usucpf)
                WHERE
                    sisid = {$_SESSION['sisid']}
                AND pfu.pflcod IN ({$arrPerfis})
                AND usi.suscod = 'A'";
        $destinatarios = $db->carregar($sql);

        //ordenando array para tratar na lista
        foreach ($destinatarios as $key => $value) {
            $arrayDestinatarios[$key]["acao"] = $value["usucpf"];
            $arrayDestinatarios[$key]["usucpf"] = $value["usucpf"];
            $arrayDestinatarios[$key]["usunome"] = $value["usunome"];
            $arrayDestinatarios[$key]["usuemail"] = $value["usuemail"];
            $arrayDestinatarios[$key]["regcod"] = $value["regcod"];
        }

        echo <<<SCRIPT
            <script language="JavaScript" src="../../includes/funcoes.js"></script>
SCRIPT;
        if (!empty($arrayDestinatarios)):
            $cabecalho = array("<input type=\"checkbox\" name=\"todos\" id=\"todos\" value=\"todos\" />", 'CPF', 'Nome', 'E-mail', 'Estado');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->addCallbackDeCampo('acao', 'addCheckboxEmail')
                ->turnOnPesquisator()
                ->setCabecalho($cabecalho)
                ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
                ->setFormOff()
                ->setDados($arrayDestinatarios);
            $mensagem = str_replace('"', "'", $_REQUEST["mensagem"]);
            echo <<<HTML

                <form class="form-horizontal" id="formularioListaUsuarios" method="post" name="formularioListaUsuarios" enctype="multipart/form-data" action="">
HTML;
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            echo <<<HTML
                    <div>
                        <input type="hidden" value="{$_REQUEST["stNomeRemetente"]}" name="stNomeRemetente" id="stNomeRemetente" />
                        <input type="hidden" value="{$_REQUEST["stEmailRemetente"]}" name="stEmailRemetente" id="stEmailRemetente" />
                        <input type="hidden" value="{$_REQUEST["assunto"]}" name="assunto" id="assunto" />
                        <input type="hidden" value="{$mensagem}" name="mensagem" id="mensagem" />
                    </div>
                </form>
HTML;
        else:
            echo <<<HTML
                <section class="alert alert-danger text-center">
                    N�o h� destinat�rios para os filtros indicados.
                </section>
HTML;
            echo <<<SCRIPT
            <script>
                $('#modal-confirm .btn-primary').remove();
            </script>
SCRIPT;
        endif;
        echo <<<SCRIPT
            <script language="JavaScript">
                $("#todos").change(function() {
                    $("input:checkbox").prop('checked', $(this).prop("checked"));
                });
            </script>
SCRIPT;
    }

    public function DisparaEmail() {
        ini_set("memory_limit", "2048M");
        set_time_limit(600);

        $arDestinatariosSelecao = array();
        if (is_array($_REQUEST["arDestinatarios"])) {
            foreach ($_REQUEST["arDestinatarios"] as $indice => $stDados) {
                // Nome
                $arDestinatariosSelecao[$indice]['usunome'] = $stDados['usunome'];
                // E-mail
                $arDestinatariosSelecao[$indice]['usuemail'] = $stDados['usuemail'];
                // CPF
                $arDestinatariosSelecao[$indice]['usucpf'] = $stDados['usucpf'];
            }
        }

        if ($_REQUEST["stNomeRemetente"] && $_REQUEST["stEmailRemetente"]) {
            $remetente['usunome'] = $_REQUEST["stNomeRemetente"];
            $remetente['usuemail'] = $_REQUEST["stEmailRemetente"];
            $remetente['usucpf'] = "";
        }

        array_push($arDestinatariosSelecao, $remetente);

        $assunto = $_REQUEST["assunto"];
        $conteudo = $_REQUEST["mensagem"];
        //----------------------------------------------------------------------------------
        # envia as mensagens
        #$mensagem = new EmailSistema();
        if (!$this->_enviar($arDestinatariosSelecao, $assunto, $conteudo, (($_SESSION["FILES"]) ? $_SESSION["FILES"] : array()), $remetente, $_SESSION["destino"], false)) {
            echo 'Ocorreu uma falha ao enviar a mensagem.';
        } else {
            echo 'Opera��o efetuada com sucesso.';
        }

        die();

    }

    protected function _enviar( array $destinatarios, $assunto, $conteudo, array $arquivos, $remetenteInformado = array(), $destinoArquivo = null, $condicao = true )
    {
        require_once APPRAIZ . "includes/Email.php";
        $objetoEmail = new Email();
        # identifica o remetente
        $remetente = $objetoEmail->pegarUsuario( $_SESSION['usucpforigem'] );
        if (!$remetente->usucpf ) {
            return false;
        }
        $objetoEmail->Host = 'smtp2.mec.gov.br';
        $objetoEmail->CharSet = 'ISO-8895-1';
        $objetoEmail->Timeout = 60;
        #$this->SMTPDebug = true;
        $objetoEmail->From     = isset( $remetenteInformado["usuemail"] ) ? $remetenteInformado["usuemail"] : $remetente->usuemail;
        $objetoEmail->FromName = isset( $remetenteInformado["usunome"]  ) ? $remetenteInformado["usunome"]  : $remetente->usunome;

        # identifica os destinat�rios
        foreach ( $destinatarios as &$destinatario ) {
            $objetoEmail->AddBCC( $destinatario["usuemail"], $destinatario["usunome"] );
        }
        # anexa os arquivos
        foreach ( $arquivos as $arquivo ) {
            if ( $arquivo["error"] == UPLOAD_ERR_NO_FILE ) {
                continue;
            }

            $objetoEmail->AddAttachment( $destinoArquivo, basename( $destinoArquivo ) );
        }

        # formata assunto, conteudo e envia a mensagem
        $objetoEmail->Subject = Email::ASSUNTO . str_replace( "\'", "'", $assunto );
        $objetoEmail->Body    = str_replace( "\'", "'", utf8_decode($conteudo));
        $objetoEmail->IsHTML( true );
        set_time_limit(180);


        if(!$objetoEmail->Send()) {
            return false;
        }

        if($condicao){
            return $objetoEmail->registrar( $remetente, $destinatarios, $assunto, $conteudo );
        }else{
            return true;
        }
    }

}