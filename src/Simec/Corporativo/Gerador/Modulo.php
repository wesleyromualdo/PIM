<?php
namespace Simec\Corporativo\Gerador;

/**
 * Description of Modulo
 *
 * @author calixto
 */
class Modulo {

    use \MecCoder\View\ViewTxtTrait;

    protected function executarCriacao($nome)
    {
        $dirMecCoder = APPRAIZ . strtolower($nome) . '/MecCoder';
        if (!is_dir($dirMecCoder)) {
            echo 'Criando diretório '.$dirMecCoder."\n";
            mkdir($dirMecCoder);
            echo 'Criando diretório '.$dirMecCoder.'/Controle'."\n";
            mkdir($dirMecCoder.'/Controle');
            echo 'Criando diretório '.$dirMecCoder.'/Visao'."\n";
            mkdir($dirMecCoder.'/Visao');
            echo 'Criando diretório '.$dirMecCoder.'/Modelo'."\n";
            mkdir($dirMecCoder.'/Modelo');
            echo 'Criando diretório '.$dirMecCoder.'/Dado'."\n";
            mkdir($dirMecCoder.'/Dado');
            $this->toView('modulo', ucfirst($nome));
            echo 'Criando trait '.$dirMecCoder.'/ControleTrait.php'."\n";
            \file_put_contents($dirMecCoder.'/ControleTrait.php', $this->getTxt('src/Simec/Visao/gerador/tplControleTrait.txt'));
            echo 'Criando UsuarioResponsabilidade '.$dirMecCoder.'/UsuarioResponsabilidade.php'."\n";
            \file_put_contents($dirMecCoder.'/UsuarioResponsabilidade.php', $this->getTxt('src/Simec/Visao/gerador/tplUsuarioResponsabilidade.txt'));
            echo 'Criando UsuarioResponsabilidade '.$dirMecCoder.'/UsuarioSessao.php'."\n";
            \file_put_contents($dirMecCoder.'/UsuarioSessao.php', $this->getTxt('src/Simec/Visao/gerador/tplUsuarioSessao.txt'));
            return true;
        }
        return false;
    }
    
    public static function gerarModulo($nome){
        $modulo = new Modulo();
        return $modulo->executarCriacao($nome);
    }
    
}