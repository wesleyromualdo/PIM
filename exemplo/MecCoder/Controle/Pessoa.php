<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace Simec\Exemplo\Controle;

use Simec\Exemplo\Dado\Pessoa as DadoPessoa;
use Simec\Exemplo\Modelo\Pessoa as ModeloPessoa;
use Simec\Exemplo\Dado\Dependente as DadoDependente;
use Simec\Exemplo\Modelo\Dependente as ModeloDependente;

/**
 * Description of Pessoa
 *
 * @author calixto
 */
class Pessoa extends \Simec\AbstractControlePublico
{

    public function finish()
    {
        
    }

    public function init()
    {
        $this
            ->js('/zimec/public/temas/simec/js/plugins/viewjs/view.js')
            ->js('/zimec/public/temas/simec/js/plugins/zoomDetect.js');
    }
    
    public function verListagemGeral(){
        $modelo = new ModeloPessoa();
        $this->toJs('pessoas', $modelo->lerSimilares(new DadoPessoa($this->post()),['nome'])->enviarVisao());
        $this->showHtml();
    }

    public function verPesquisa()
    {
        //echo (new \Simec\Corporativo\Gerador\Dado())->gerar('territorios', 'estado');die;
        $modelo = new ModeloPessoa();
        $this->toJs('filtro', $filtro);
        $this->toJs('estados', $modelo->lerSimilares(new \Simec\Territorios\Dado\Estado()));
        //$this->toJs('pessoas', $modelo->lerSimilares(new DadoPessoa($this->post()),['nome'], new \Simec\Pagina($this->get('page'),5))->enviarVisao());
        $this->showHtml();
    }
    
    public function pesquisar(){
        $pagina = new \Simec\Pagina($this->get('page'));
        $modelo = new ModeloPessoa();
        $this->toJson('pessoas',$modelo->lerSimilares(new DadoPessoa($this->post()),['nome'], $pagina)->enviarVisao())
            ->toJsonPage($pagina)
            ->showJson();
    }

    public function verEdicao()
    {
        if ($this->get('id')) {
            $this->toJs('pessoa', (new ModeloPessoa())->lerObjeto(new DadoPessoa($this->get()))->enviarVisao());
            $this->toJs('dependentes', (new ModeloDependente())->lerSimilares(new DadoDependente(['idPessoa' => $this->get('id')])));
        }
        $this->toJs('estados', (new ModeloPessoa())->lerSimilares(new \Simec\Territorios\Dado\Estado()));
        $this->showHtml();
    }

    public function salvar()
    {
        try {
            $modelo = new ModeloPessoa();
            $modelo->gravar((new DadoPessoa($this->post()))->enviarBanco());
            $modelo->validarTransacao();
            $this->registrarMensagem('SIMEC', 'Pessoa salva com sucesso.');
            if($this->post('id')){
                header('location:?modulo=exemplo/controle/pessoa/ver-pesquisa');
            }else{
                header('location:?modulo=exemplo/controle/pessoa/ver-edicao');
            }
        } catch (Exception $ex) {
            (new ModeloPessoa())->desfazerTransacao();
            header('location:?modulo=exemplo/controle/pessoa/ver-edicao');
        }
    }

    public function excluir()
    {
        try {
            if ($this->get('id')) {
                (new ModeloPessoa($this->get()))
                    ->excluirColecao((new ModeloDependente())->lerSimilares(new DadoDependente(['idPessoa' => $this->get('id')])))
                    ->excluir()
                    ->validarTransacao();
            }
            header('location:?modulo=exemplo/controle/pessoa/ver-pesquisa');
        } catch (\Exception $ex) {
            (new ModeloPessoa())->desfazerTransacao();
            header('location:?modulo=exemplo/controle/pessoa/ver-edicao');
        }
    }
}
