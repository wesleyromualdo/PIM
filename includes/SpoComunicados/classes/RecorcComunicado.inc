<?php
include_once APPRAIZ . "recorc/classes/model/Anexogeralperfil.class.inc";
include_once APPRAIZ . "recorc/classes/model/Anexogeralunidade.class.inc";
include_once APPRAIZ . "includes/SpoComunicados/Usuarioresponsabilidade.class.inc";

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of RecorcComunicado
 *
 * @author LindalbertoFilho
 */
class RecorcComunicado extends Modelo{

    protected $stNomeTabela = 'recorc.anexogeral';
    protected $arChavePrimaria = array('angid',);
    protected $arAtributos = array(
        'angid'  => NULL,
        'angdsc' => NULL,
        'arqid'  => NULL,
        'angtip' => NULL,
        'capid'  => NULL,
        'angmsg' => NULL,
        'angdtinicio' => NULL,
        'angdtfim' => NULL,
        'angstatus' => NULL,
        'angano' => NULL
    );

    protected $_arExtencoes = array('pdf','xls','xlsx');
    /**
     *
     * @var Simec_Helper_FlashMessage
     */
    protected $_message;

    public function __construct() {
        $this->_message = new Simec_Helper_FlashMessage(MODULO.'/comunicados');
    }

    private function hasErro(){
        $hasErro = false;
        if((new DateTime($_POST['dataini']))  > (new DateTime($_POST['datafim']))){
            $this->_message->addMensagem('Data de inicio do período é maior que a data final.', Simec_Helper_FlashMessage::ERRO);
            $hasErro = true;
        }

        if(count($_POST['perfil']) <= 0){
            $this->_message->addMensagem('É necessário selecionar pelo menos um perfil.', Simec_Helper_FlashMessage::ERRO);
            $hasErro = true;
        }

        if(count($_POST['unidadeorcamentaria']) <= 0){
            $this->_message->addMensagem('É necessário selecionar pelo menos uma unidade orçamentária.', Simec_Helper_FlashMessage::ERRO);
            $hasErro = true;
        }

        return $hasErro;
    }

    public function cadastrar()
    {
        if($this->hasErro()){
            return false;
        }

        $file = new FilesSimec(null, null, 'recorc');
        if($_FILES ["file"] [size] > 0){
            /* Verificando extensão. */
            if (array_search(
                    strtolower(end(explode('.', $_FILES['file']['name']))),
                    $this->_arExtencoes) === false) {
                $this->_message->addMensagem('Por favor, envie arquivos somente com as extensões permitidas: <code>'.  implode(',', $this->_arExtencoes) .'</code>.', Simec_Helper_FlashMessage::AVISO);
            } else {
                if($file->setUpload($_FILES ['file'] ['name'], NULL, false)){
                    $this->_message->addMensagem('Arquivo cadastrado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
                }else{
                    $this->_message->addMensagem('Falha ao cadastrar arquivo.', Simec_Helper_FlashMessage::ERRO);
                }
            }
        }

        if(!is_null($_POST['mensagem']) || !is_null($_POST['dataini']) || !is_null($_POST['datafim'])){
            $Atributos = array(
                "angdsc"       => "'{$_POST['nomeArquivo']}'",
                "angtip"       => "'CO'",
                "angano" => "'{$_SESSION['exercicio']}'",
                "angmsg" => "'{$_POST['mensagem']}'",
                "angdtinicio" => $_POST['dataini']?$_POST['dataini']:NULL,
                "angdtfim" => $_POST['datafim']?$_POST['datafim']:NULL,
                "arqid" => $file->getIdArquivo()
            );

            $this->popularDadosObjeto($Atributos);

            $id = $this->salvar();
            if($id != false){
                $perfils = $_POST['perfil']?$_POST['perfil']:array();
                $unidades = $_POST['unidadeorcamentaria']?$_POST['unidadeorcamentaria']:array();

                $anexoGeralPerfil = new Recorc_Model_Anexogeralperfil();
                foreach ($perfils as $perfil){
                    $anexoGeralPerfil->popularDadosObjeto(['angid'=>$id,'pflcod'=>$perfil])->salvar();
                }

                $anexoGeralUnidade = new Recorc_Model_Anexogeralunidade();
                if(($i = array_search("-1", $unidades))){
                    unset($unidades[$i]);
                }
                foreach ($unidades as $unidade){
                    $anexoGeralUnidade->popularDadosObjeto(['angid'=>$id,'unocod'=>$unidade])->salvar();
                }
            }

            if($this->commit()){
                $this->_message->addMensagem('Comunicado cadastrado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
                return true;
            }else{
                $this->_message->addMensagem('Falha ao cadastrar o comunicado.', Simec_Helper_FlashMessage::ERRO);
                return false;
            }
        }
    }

    public function atualizar()
    {
        if($this->hasErro()){
            return false;
        }

        if(!empty($_POST['angid'])) {
            $perfils = (new Recorc_Model_Anexogeralperfil())->obterPerfilComunicado($_POST['angid']);
            $uos = (new Recorc_Model_Anexogeralunidade())->obterUOComunicado($_POST['angid']);

            $perfilsAlterados = $_POST['perfil'];
            foreach ($perfils as $pfl) {
                $key = array_search($pfl['pflcod'], $perfilsAlterados);
                if ($key === false) {
                    (new Recorc_Model_Anexogeralperfil())->popularDadosObjeto(['angid' => $_POST['angid'], 'pflcod' => $pfl['pflcod']])->excluir();
                } else {
                    unset($perfilsAlterados[$key]);
                }
            }

            foreach($perfilsAlterados as $pflIncluir){
                (new Recorc_Model_Anexogeralperfil())->popularDadosObjeto(['angid' => $_POST['angid'], 'pflcod' => $pflIncluir])->salvar();
            }

            $uoAlteradas = $_POST['unidadeorcamentaria'];
            if(($i = array_search("-1", $uoAlteradas))){
                unset($uoAlteradas[$i]);
            }
            foreach ($uos as $uo){
                $key = array_search($uo['unicod'], $uoAlteradas);
                if ($key === false) {
                     (new Recorc_Model_Anexogeralunidade())->popularDadosObjeto(['angid' => $_POST['angid'], 'unocod' => $uo['unicod']])->excluir();
                } else {
                    unset($uoAlteradas[$key]);
                }
            }

            foreach($uoAlteradas as $uoIncluir){
                (new Recorc_Model_Anexogeralunidade())->popularDadosObjeto(['angid' => $_POST['angid'], 'unocod' => $uoIncluir])->salvar();
            }

            $file = new FilesSimec(null, null, 'recorc');
            if ($_FILES ["file"] [size] > 0) {
                /* Verificando extensão. */
                if (array_search(
                        strtolower(end(explode('.', $_FILES['file']['name']))),
                        $this->_arExtencoes) === false
                ) {
                    $this->_message->addMensagem('Por favor, envie arquivos somente com as extensões permitidas: <code>' . implode(',', $this->_arExtencoes) . '</code>.', Simec_Helper_FlashMessage::AVISO);
                } else {
                    $file->setUpload($_FILES ['file'] ['name'], NULL, false);
                }
            }

            $Atributos = array(
                "angid" => "{$_POST['angid']}",
                "angdsc" => "{$_POST['nomeArquivo']}",
                "angtip" => "CO",
                "angano" => "{$_SESSION['exercicio']}",
                "angmsg" => "{$_POST['mensagem']}",
                "angdtinicio" => $_POST['dataini'] ? $_POST['dataini'] : NULL,
                "angdtfim" => $_POST['datafim'] ? $_POST['datafim'] : NULL,
            );

            $file->getIdArquivo() ? $Atributos["arqid"] = $file->getIdArquivo() : NULL;
            $this->popularDadosObjeto($Atributos);
            $this->alterar(['angdtinicio', 'angdtfim']);

            if($file->getIdArquivo()){
                $file->setPulaTableEschema(true);
                $file->setRemoveUpload($_POST['arqid']);
            }


            if ($this->commit()) {
                $this->_message->addMensagem('Arquivo atualizado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
                return true;
            } else {
                $this->_message->addMensagem('Falha ao atualizar arquivo.', Simec_Helper_FlashMessage::ERRO);
                return false;
            }
        }

        return false;
    }

    public function verificaValor($valor){
        if(is_array($valor)){
            $retorno = true;
            foreach($valor as $key => $val){
                if(!$val || $val == null || trim($val) == ''){
                    trigger_error( "O ATRIBUTO ({$key}) NÃO ESTÁ POPULADO!" );
                    $retorno = false;
                }
            }
            return $retorno;
        }
    }

    public function deletar(){
        $sql = "UPDATE {$this->stNomeTabela} set angstatus = 'I' WHERE angid = {$this->arAtributos['angid']}";
        $this->executar($sql);
        if($this->commit()){
            $this->_message->addMensagem('Comunicado deletado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
            return true;
        }else{
            $this->_message->addMensagem('Falha ao deletar comunicado.', Simec_Helper_FlashMessage::ERRO);
            return false;
        }
    }

    public function listaInicial(){
        $sql = <<<HTML
            SELECT DISTINCT 
                arq.arqid,
                arq.arqnome,
                arq.arqextensao,
                con.angdsc
            FROM {$this->stNomeTabela} con
            LEFT JOIN public.arquivo arq on con.arqid = arq.arqid
            LEFT JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
            WHERE con.angtip = 'CO'
            AND angano = '{$_SESSION['exercicio']}' AND angstatus = 'A'
            ORDER BY angdsc
HTML;

        return $this->carregar($sql);
    }

    public function listarComunicados($condicao = null){
        $sql =  "select distinct ang.angid, ang.arqid,
                    '' as \"Perfil\",
                    '' as  \"Unidade\", 
                    to_char(ang.angdtinicio, 'dd-MM-yyyy')||' / '||to_char(ang.angdtfim, 'dd-MM-yyyy') as \"Periodo\",
                    ang.angdsc as \"nomeArquivo\",
                    ang.angmsg as \"Mensagem\"
                from recorc.anexogeral ang
                join recorc.anexogeralperfil angp using (angid)
                join recorc.anexogeralunidade angu on ang.angid = angu.angid
                left join public.arquivo arq using (arqid) WHERE  angstatus = 'A'";
        $sql .= $condicao?$condicao:"";

        return $this->preparaDadosParaExibicao($this->carregar($sql));
    }


    public function obterComunicados($usucpf){
        $arrUsuResponsabilidade = (new Model_Usuarioresponsabilidade ())->carregar("select pflcod, unicod from recorc.usuarioresponsabilidade where usucpf like '{$usucpf}'");
        $arrUsuResponsabilidade = $arrUsuResponsabilidade?$arrUsuResponsabilidade:array();
        $pflUsu = [];
        $uoUsu = [];
        foreach($arrUsuResponsabilidade as $usuResponsabilidade){
            $pflUsu[] = $usuResponsabilidade['pflcod'];
            $uoUsu[] = "'".$usuResponsabilidade['unicod']."'";
       }

       $pflUsu = implode(',',$pflUsu);
        $uoUsu = implode(',',$uoUsu);

        if($pflUsu != ''  && $uoUsu != '') {
            $sql = "select distinct ang.angid, ang.angdsc, ang.angmsg, ang.arqid
                from recorc.anexogeral ang 
                join recorc.anexogeralperfil angp using (angid)
                join recorc.anexogeralunidade angu on ang.angid = angu.angid
                left join public.arquivo arq using (arqid)
                where angp.pflcod in ({$pflUsu}) and angu.unocod in ({$uoUsu}) and 
                ang.angstatus = 'A' 
                and  ((ang.angdtinicio <= now() and ang.angdtinicio <= now())
                OR (ang.angdtinicio is null and ang.angdtinicio is null ))";
            return $this->carregar($sql);
        }
        return false;
    }

    public function listarComunicadoEditar($angid){
        $sql =  "select distinct ang.angid, ang.arqid,
                    '' as \"Perfil\",
                    '' as  \"Unidade\", 
                    to_char(ang.angdtinicio, 'yyyy-MM-dd')||' / '||to_char(ang.angdtfim, 'yyyy-MM-dd') as \"Periodo\",
                    ang.angdsc as \"nomeArquivo\",
                    ang.angmsg as \"Mensagem\"
                from recorc.anexogeral ang
                join recorc.anexogeralperfil angp using (angid)
                join recorc.anexogeralunidade angu on ang.angid = angu.angid
                left join public.arquivo arq using (arqid) WHERE  angstatus = 'A' and ang.angid = {$angid}";

        $res = $this->carregar($sql);

        $perfils = (new Recorc_Model_Anexogeralperfil())->obterPerfilComunicado($_POST['angid']);

        $res[0]['Perfil'] = [];
        foreach($perfils as $id){
            $res[0]['Perfil'][] = $id['pflcod'];
        }

        $uos = (new Recorc_Model_Anexogeralunidade())->obterUOComunicado($_POST['angid']);

        $res[0]['Unidade'] = [];
        foreach($uos as $id) {
            $res[0]['Unidade'][] = $id['unicod'];
        }

        return $res[0];
    }


    public function buscarDescricao(){
        $query = <<<DML
            SELECT 
                con.angdsc 
            FROM {$this->stNomeTabela} con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
            WHERE con.arqid = {$this->arAtributos['arqid']}
            ORDER BY angtip
DML;
        return $this->pegaUm($query);
    }


    private function preparaDadosParaExibicao($comunicados)
    {
        $comunicados = is_array($comunicados) ? $comunicados : array();
        $dados = [];
        foreach ($comunicados as $comunicado) {
            $perfis = (new Recorc_Model_Anexogeralperfil())->obterPerfilComunicado($comunicado['angid']);
            $str = [];
            foreach ($perfis as $pfl) {
                $str[] = $pfl['pfldsc'];
            }
            $comunicado['Perfil'] = implode(' | ', $str);

            $uos = (new Recorc_Model_Anexogeralunidade())->obterUOComunicado($comunicado['angid']);
            $str = [];
            foreach ($uos as $uo) {
                $str[] = $uo['unidsc'];
            }
            $comunicado['Unidade'] = implode(' | ', $str);

            $comunicado['Arquivo'] = "<a href='recorc.php?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid={$comunicado['arqid']}'>{$comunicado['nomeArquivo']}</a>";
            if ($comunicado['Periodo'] == "") {
                $comunicado['Periodo'] = 'Não definido';
            }
            $dados[] = $comunicado;

        }
        if (isset($dados['Periodo'])) {
            if ($dados['Periodo'] == "") {
                $dados['Periodo'] = [""];
            }
        }
        return $dados;
    }

}
