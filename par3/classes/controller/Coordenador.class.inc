<?php

class Par3_Controller_Coordenador
{
    private $model;
    private $url;

    public function __construct()
    {
        $this->url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=coordenador';
        $this->model = new Par3_Model_Coordenador();
        $this->modelPrograma = new Par3_Model_Coordenador();
    }

    public function salvar($dados){

        //Verifica se já existe coordenador cadastrado com este nome
        if($this->model->existeNomeCoordenador($dados['coonome'])){
            simec_redirecionar($this->url, 'error', "Já existe Coordenador cadastrado com o nome informado!");
        }

        try {
            if (empty($dados)) {
                throw new Exception('Dados não informados');
            }

            $this->model->popularDadosObjeto();

            //inseri o upload
            $arqid = $this->inserirUpload();
            $this->model->arqid = $arqid;

            $this->model->salvar();

            if (!$this->model->commit()) {
                throw new Exception('Erro ao tentar salvar coordenador');
            }

            simec_redirecionar($this->url, 'success');
        } catch (Exception $e) {
            simec_redirecionar($this->url, 'error', $e->getMessage());
        }
    }

    public function editar($dados){

        //Verifica se já existe coordenador cadastrado com este nome
        if($this->model->existeNomeCoordenador($dados['coonome'], $dados['cooid'])){
            simec_redirecionar($this->url, 'error', "Já existe Coordenador cadastrado com o nome informado!");
        }

        try {

            $this->model->popularDadosObjeto($dados);

            //insere o upload caso tenha um novo
            $arqid = $this->inserirUpload();
            $this->model->arqid = $arqid;

            $this->model->salvar();
            if (!$this->model->commit()) {
                throw new Exception('Erro ao tentar salvar coordenador');
            }

            /**
             * Exclui o anexo atual, caso tenho selecionado outro.
             */
            if (isset($dados['idsExcluirAnexo'])) {
                foreach ($dados['idsExcluirAnexo'] as $idArquivo) {
                    $this->apagarArquivo($idArquivo);
                }
            }



            simec_redirecionar($this->url, 'success');
        } catch (Exception $e) {
            simec_redirecionar($this->url, 'error', $e->getMessage());
        }
    }

    /**
     * Função para inserir upload.
     *
     * @param $otdid
     * @return mixed
     */
    public function inserirUpload()
    {
        $file = new FilesSimec();
        $file->setUpload('Assinatura Coordenador', null, false);
        $dados['arqid'] = $file->getIdArquivo();

        return $dados['arqid'];
    }

    public function apagarArquivo($arqId)
    {
        $file = new FilesSimec();
        $file->setPulaTableEschema(true);
        $file->setRemoveUpload($arqId);
        return true;
    }


    public function formPesquisa(){
        global $simec;

        #somente onkeyup, segurando a tecla ele deixa passar. somente onkeypress ele não retira o caracter na mesma hora, permitindo que seja feita a pesquisa. Por isso deixei os 2
        echo $simec->input('cooid', 'Código', $_POST['cooid'], array('onKeyUp'=>'this.value=somenteNumeros(this.value)', 'onkeypress'=>'this.value=somenteNumeros(this.value)'), array('maxlength' => '255', 'input-size' => '2'));
        echo $simec->input('coonome', 'Coordenador', $_POST['coonome'], array(), array('maxlength' => '255', 'input-size' => '7'));

    }

    public function formSalvar(){
        global $simec;

        if($this->model->existeAtivo()){
            $ativo = 1;
        }else{
            $ativo = 0;
        }

        echo "<input type='hidden' name='existeAtivo' id='existeAtivo' value='{$ativo}'>";
        echo "<input type='hidden' name='possuiAnexo' id='possuiAnexo' value=''>";
        echo "<input type='hidden' name='requisicao' id='requisicao' value=''>";
        echo $simec->input('cooid', 'Código', null, array('readonly' => 'readonly'), array('maxlength' => '255', 'input-size' => '3'));
        echo $simec->input('coonome', 'Nome', null, array('required' => 'required'), array('maxlength' => '255', 'input-size' => '9'));
        echo $simec->data('coodtainclusao', 'Data', null, array(), array('maxlength' => '255', 'input-size' => '9'));

        echo "<div class=\"form-group\">
                    <label class=\"col-md-offset-1 col-lg-offset-1 col-sm-2 control-label\">
                        Anexo: *
                    </label>
                    <div class=\"col-md-9  col-md-9 col-lg-9\">
                        <input type=\"file\" name=\"anexo\" id=\"anexo\" class=\"form-control\" style=\"height: auto !important;\"/>
                    </div>
                </div>";
        echo "<div id=\"listaAnexo\"></div>";

        echo $simec->textarea('coobservacao', 'Observação', null, array(), array('maxlength' => '255', 'input-size' => '9'));
        echo $simec->radio('coostatus', 'Situação', null, ['A' => 'Ativo', 'I' => 'Inativo'], null, array('type' => 'radio radio-info radio', 'style' => 'inline'));

    }

    public function recuperarPorId($dados){
        $dados = $this->model->recuperarPorId($dados['cooid']);
        echo simec_json_encode($dados);
    }

    public function listar($dados){
        $sql = $this->model->montaSQLLista($dados);

        $cabecalho = array('Código', 'Coordenador', 'Arquivo', 'Observação', 'Inserido por:', 'Data', 'Situação');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas('arqid');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('arqnome', 'linkArquivoAssinaturaCoordenador');
        $listagem->addAcao('edit', 'modalEditar', array('titulo' => 'Alterar'));
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

}