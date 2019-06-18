<?php

class Alerta extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.alerta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("aid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aid' => null,
        'titulo' => null,
        'mensagem' => null,
        'situacao' => null,
        'condicao' => null,
        'msg_condicao' => null,
        'pos_acao' => null,
        'dtcriacao' => null,
        'contato' => null,
        'cabecalho' => null,
        'registro_atividade' => null,
        'agid' => null,
    );

    private $condicional = false;

    protected $arObras = array();

    /**
     * @return array|bool array(id (id do alerta),obras (classe ObraVO), mensagem (mensagem do alerta), mensagem_condicao (mensagem da condição))
     */
    public function verificaExistencia()
    {
        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

            $alertaPerfil = new AlertaPerfil();
            $alertas = $alertaPerfil->buscaAlerta();
            if(!$alertas) return false;
            $saida = array();

            foreach($alertas as $alerta) {
                $this->carregarPorId($alerta['aid']);
                $this->arObras = explode(',,',$alerta['obrid_list']);
                $this->trabalhaArrayObras();
                $condicao = $this->checaCondicao();
                $alertaArquivo = new AlertaArquivo();
                $arquivos = $alertaArquivo->buscaArquivosPorAlerta($alerta['aid']);
                $alertaObraLido = new AlertaObraLido();
                $alertaObraLido->salvarPreLeitura($this->arObras);
                if($this->agid) {
                    $assinaturaGestor = new AssinaturaGestor();
                    $dadosAssinaturaGestor = $assinaturaGestor->buscaAssinatura($this->agid);
                }
                $saida[] = array('aid' => $alerta['aid'],
                    'obras' => $this->arObras,
                    'titulo' => $this->titulo,
                    'mensagem' => $this->mensagem,
                    'mensagem_condicao' => ($condicao != '' ? $condicao : $this->msg_condicao),
                    'arquivos' => $arquivos,
                    'condicional' => $this->condicional,
                    'exibe_cabecalho' => ($this->cabecalho == 'A' ? true : false),
                    'contato' => $this->contato,
                    'ra' => $this->registro_atividade,
                    'assinatura_gestor' => $dadosAssinaturaGestor,
                    'pos_acao' => $this->pos_acao);
                #$this->pos_acao;
                #$this->msg_condicao;
            }
            return $saida;
        }
    }

    protected function trabalhaArrayObras()
    {
        $novo = array();
        foreach($this->arObras as $obra) {
            $obraVO = new ObraVO();
            $saida = explode('|@|',$obra);
            $obraVO->obrid = $saida[0];
            $obraVO->obrnome = $saida[1];
            $obraVO->estuf = $saida[2];
            $obraVO->mundescricao = $saida[3];
            $obraVO->tpodsc = $saida[4];
            $obraVO->aoid = $saida[5];
            $obraVO->aid = $this->aid;
            $novo[] = $obraVO;
        }
        $this->arObras = $novo;
    }

    protected function checaCondicao()
    {
        if($this->condicao) {
            $this->condicional = true;
            $alertaCondicao = new AlertaCondicao();
            $alertaCondicao->arObras = $this->arObras;
            $saida = $alertaCondicao->executaFuncao($this->condicao);
            $this->arObras = $alertaCondicao->arObras;
            return $saida;
        }
    }

    protected function checaPosAcao($posAcao,$dados)
    {
        $posAcaoAlerta = new PosAcaoAlerta();
        /*$dados['ra']
        $dados['id_alerta']
        $dados['pos_acao']
        $dados['id_alerta_obra']  array
        $dados['obrid']  array
        $dados['obrnome']  array*/
        $posAcaoAlerta->aid = $dados['aid'];
        return $posAcaoAlerta->executaFuncao($posAcao);
    }

    public function salvarAlerta($dados)
    {
        try{
            if($dados['aid']) {
                $sql = "
                    UPDATE {$this->stNomeTabela} SET titulo = '".$dados['titulo']."',
                    mensagem = '".$dados['mensagem']."',
                    condicao= '".$dados['condicao']."',
                    msg_condicao = '".$dados['msg_condicao']."',
                    pos_acao = '".$dados['pos_acao']."',
                    contato = '".$dados['contato']."',
                    cabecalho = '".$dados['cabecalho']."',
                    registro_atividade = '".$dados['registro_atividade']."',
                    agid = ".$dados['agid']."
                    WHERE aid = ".$dados['aid']."
                ";
                $this->executar($sql);
                $this->commit();
                $id = $dados['aid'];
            } else {
                $sql = "
                    INSERT INTO {$this->stNomeTabela} (titulo,mensagem,situacao,condicao,msg_condicao,pos_acao,contato,cabecalho,registro_atividade,agid)
                    VALUES ('".$dados['titulo']."','".$dados['mensagem']."','A','".$dados['condicao']."','".$dados['msg_condicao']."','".$dados['pos_acao']."','".$dados['contato']."','".$dados['cabecalho']."','".$dados['registro_atividade']."',".$dados['agid'].")
                    RETURNING aid;
                ";
                $id = $this->pegaUm($sql);
                $this->commit();
            }
            return $id;
        } catch (Exception $e) {
            return false;
        }
    }

    public function salvarAcao($dados)
    {
        try{
            if($dados['pos_acao']) {
                $this->checaPosAcao($dados['pos_acao'], $dados['aid']);
            }
            return true;
        } catch (Exception $e) {
            return false;
        }
    }

    /**
     * Registra na tabela de atividades do envio do e-mail
     */
    protected function registraAtividade($arqidConteudo) {
        // Monta o arquivo com corpo
        $sql = "select * from obras2.tipoemail where temid = $this->temid";
        $tipo = $this->pegaLinha($sql);

        $arDado = array();

        $arDado['rgaid'] = $this->rgaid;
        $arDado['arqid'] = $arqidConteudo;
        $arDado['obrid'] = $this->obrid;
        $arDado['rgaautomatica'] = 'true';
        $arDado['rgadscsimplificada'] = 'E-mail enviado (' . $tipo['temnome'] . ')';
        $arDado['rgadsccompleta'] = 'E-mail enviado (' . $tipo['temnome'] . ') para: ' . $this->destinatariosToString($this->getDestinatarios());


        if (empty($arDado['arqid']))
            $arDado['arqid'] = 'NULL';

        $sql = "INSERT INTO obras2.registroatividade (rgaid, arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                  {$arDado['rgaid']},
                  {$arDado['arqid']},
                  {$arDado['obrid']},
                  {$arDado['rgaautomatica']},
                  '{$arDado['rgadscsimplificada']}',
                  '{$arDado['rgadsccompleta']}'
                  )";

        $this->executar($sql);

        return $this->rgaid;
    }


}
