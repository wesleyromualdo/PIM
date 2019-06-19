<?php
namespace Simec\Corporativo\Dado;

class EstruturaCampo extends \Simec\AbstractDado
{

    public $esquema;
    public $tabela;
    public $campo;
    public $tipo;
    public $tipoDado;
    public $obrigatorio;
    public $tamanho;
    public $descricao;
    public $campoPk;
    public $constraint;
    public $esquemaFk;
    public $tabelaFk;
    public $campoFk;
    public $uk;

    public function carregar($ar)
    {
        parent::carregar($ar);
        if($this->tipoDado == 'numerico'){
            $this->tamanho = $this->tamanho ? $this->tamanho : 8;
        }
        return $this;
    }

}
