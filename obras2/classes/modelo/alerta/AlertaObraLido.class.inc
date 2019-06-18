<?php

class AlertaObraLido extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.alerta_obra_lido";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("aolid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aolid' => null,
        'aid' => null,
        'aoid' => null,
        'usucpf' => null,
        'acao' => null,
        'dtacao' => null,
    );

    public function salvarAlertaLido($dados, $acao = 'L')
    {
        try{
            foreach($dados['aoid'] as $key => $alertaObra) {
                $alertaObraLido = new AlertaObraLido();
                $alertaObraLido->usucpf = $_SESSION['usucpf'];
                $alertaObraLido->acao = $acao;
                $alertaObraLido->aid = $dados['aid'];
                $alertaObraLido->aoid = $alertaObra;
                $alertaObraLido->salvar();
            }
            $this->commit();
            return true;
        } catch(Exception $e) {
            return false;
        }
    }

    public function salvarPreLeitura($arObras)
    {
        try{
            foreach($arObras as $obra) {
                $alertaObraLido = new AlertaObraLido();
                $alertaObraLido->usucpf = $_SESSION['usucpf'];
                $alertaObraLido->acao = 'L';
                $alertaObraLido->aid = $obra->aid;
                $alertaObraLido->aoid = $obra->aoid;
                $alertaObraLido->salvar();
            }
            $this->commit();
            return true;
        } catch(Exception $e) {
            return false;
        }
    }
}